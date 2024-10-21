#include "pch.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

#include "ResourceMgr.h"
#include "PathMgr.h"

#include "GLTFLoader.h"
namespace ff7r
{
	using namespace Microsoft::glTF;

	vec4 ColorToVec4(Color4 _color)
	{
		return vec4(_color.r, _color.g, _color.b, _color.a);
	}

	vec3 ColorToVec3(Color3 _color)
	{
		return vec3(_color.r, _color.g, _color.b);
	}

	GLTFLoader::GLTFLoader()
		: resource_reader(nullptr)
	{
	}

	GLTFLoader::~GLTFLoader()
	{
		delete resource_reader;

		for (int i = 0; i < joints.size(); i++)
		{
			if (joints[i])
			{
				delete joints[i];
				joints[i] = nullptr;
			}
		}

		for (int i = 0; i < anim_clips.size(); i++)
		{
			if (anim_clips[i])
			{
				delete anim_clips[i];
				anim_clips[i] = nullptr;
			}
		}
	}

	void GLTFLoader::LoadGLTF(const path& _path)
	{		
		gltf_path = _path;
		path _full_path = PathMgr::GetInst()->GetContentPath() + _path.wstring();
		path _file_name = _full_path.filename();

		std::shared_ptr<StreamReader> _stream_reader = std::make_shared<StreamReader>(_full_path.parent_path());
		resource_reader = new GLTFResourceReader(_stream_reader);

		std::shared_ptr<std::istream> _input_stream = _stream_reader->GetInputStream(_file_name.u8string()); // Pass a UTF-8 encoded filename to GetInputString

		std::string _manifest;
		std::stringstream _manifest_stream;

		// Read the contents of the glTF file into a string using a std::stringstream
		_manifest_stream << _input_stream->rdbuf();
		_manifest = _manifest_stream.str();

		document = Deserialize(_manifest);

		LoadSkeleton();
		LoadAnimation();
		CalcWorldTransJoint();
		CreateAnimation();
		LoadMesh();
		LoadMaterial();
		CreateMaterial();
	}

	void GLTFLoader::LoadSkeleton()
	{
		for (const auto& skin : document.skins.Elements())
		{
			joints.resize(document.nodes.Elements().size() - 1);
			LoadChlidSkeleton(skin.skeletonId, "0", -1);
		}

		for (const auto& skin : document.skins.Elements())
		{
			const Accessor& inverse_bind_matrices_accessor = document.accessors.Get(skin.inverseBindMatricesAccessorId);
			const vector<float>& inverse_bind_matrices = resource_reader->ReadBinaryData<float>(document, inverse_bind_matrices_accessor);

			// 각 조인트에 대해 인버스 바인드 매트릭스 적용
			for (int i = 0; i < joints.size(); ++i)
			{
				for (int j = 0; j < 16; j++)
				{
					joints[i]->mat_inverse.m[j / 4][j % 4] = inverse_bind_matrices[i * 16 + j];
				}
			}
		}
	}

	void GLTFLoader::LoadChlidSkeleton(string _joint_id, string _parent_id, int _depth)
	{
		const Node& joint_node = document.nodes.Get(_joint_id);

		Joint* _joint = new Joint;
		_joint->name = wstring(joint_node.name.begin(), joint_node.name.end());
		_joint->depth = _depth++;
		_joint->parent_id = stoi(_parent_id) - 1;


		_joint->pos = vec3(joint_node.translation.x, joint_node.translation.y, joint_node.translation.z);
		_joint->rotation = Quater(joint_node.rotation.x, joint_node.rotation.y, joint_node.rotation.z, joint_node.rotation.w);
		_joint->scale = vec3(joint_node.scale.x, joint_node.scale.y, joint_node.scale.z);


		_joint->mat_joint = Matrix::CreateScale(_joint->scale) *
			Matrix::CreateFromQuaternion(_joint->rotation) *
			Matrix::CreateTranslation(_joint->pos);

		if (_joint->parent_id >= 0)
		{
			_joint->mat_joint = _joint->mat_joint * joints[_joint->parent_id]->mat_joint;
		}

		if (joint_node.scale.x != 1.0f
			|| joint_node.scale.y != 1.0f
			|| joint_node.scale.z != 1.0f)
		{
			// 스케일이 1이 아니다 비상
			assert(nullptr);
		}

		joints[stoi(_joint_id) - 1] = _joint;

		for (int i = 0; i < joint_node.children.size(); i++)
		{
			LoadChlidSkeleton(joint_node.children[i], _joint_id, _depth);
		}
	}

	void GLTFLoader::LoadAnimation()
	{
		for (const auto& animation : document.animations.Elements())
		{
			AnimClip* _ani_clip = new AnimClip;
			_ani_clip->name = wstring(animation.name.begin(), animation.name.end());
			_ani_clip->start_time = 0.0f;
			_ani_clip->key_frames.resize(joints.size());

			for (int i = 0; i < animation.channels.Size(); i++)
			{
				const auto& sampler = animation.samplers.Elements()[i];
				const auto& channel = animation.channels.Elements()[i];

				const Accessor& input_accessor = document.accessors.Get(sampler.inputAccessorId);
				const Accessor& output_accessor = document.accessors.Get(sampler.outputAccessorId);

				const Node& node = document.nodes.Get(channel.target.nodeId);

				const vector<float>& keyframe_times = resource_reader->ReadBinaryData<float>(document, input_accessor);

				UINT _frame_size = keyframe_times.size();
				UINT _anim_id = stoi(animation.id);

				int _joint_id = stoi(node.id) - 1;
				Joint* joint = joints[_joint_id];
				
					
				if (_ani_clip->key_frames[_joint_id].size() == 0)
				{
					_ani_clip->key_frames[_joint_id].resize(_frame_size);
					
					//joint->ani_frame[_anim_id].resize(_frame_size);
					_ani_clip->frame_times.resize(_frame_size);
					_ani_clip->end_frame = _frame_size;
					_ani_clip->end_time = keyframe_times[_frame_size - 1];
					for (UINT frame_idx = 0; frame_idx < _frame_size; frame_idx++)
					{
						_ani_clip->frame_times[frame_idx] = keyframe_times[frame_idx];
						_ani_clip->key_frames[_joint_id][frame_idx].time = keyframe_times[frame_idx];
						_ani_clip->key_frames[_joint_id][frame_idx].interpolation_type = sampler.interpolation;
					}
				}

				if (channel.target.path == TARGET_TRANSLATION)
				{
					const auto& position_data = resource_reader->ReadBinaryData<float>(document, output_accessor);

					for (UINT frame_idx = 0; frame_idx < _frame_size; frame_idx++)
					{
						KeyFrame& _key_frame = _ani_clip->key_frames[_joint_id][frame_idx];
						_key_frame.pos.x = position_data[frame_idx * 3];
						_key_frame.pos.y = position_data[frame_idx * 3 + 1];
						_key_frame.pos.z = position_data[frame_idx * 3 + 2];
					}
				}
				else if (channel.target.path == TARGET_ROTATION)
				{
					const auto& rotation_data = resource_reader->ReadBinaryData<float>(document, output_accessor);

					for (int frame_idx = 0; frame_idx < _frame_size; frame_idx++)
					{
						KeyFrame& _key_frame = _ani_clip->key_frames[_joint_id][frame_idx];
						_key_frame.rotation.x = rotation_data[frame_idx * 4];
						_key_frame.rotation.y = rotation_data[frame_idx * 4 + 1];
						_key_frame.rotation.z = rotation_data[frame_idx * 4 + 2];
						_key_frame.rotation.w = rotation_data[frame_idx * 4 + 3];
					}
				}
			}
			anim_clips.push_back(_ani_clip);
		}
	}

	void GLTFLoader::CalcWorldTransJoint()
	{
		for (auto& clip : anim_clips)
		{
			for (UINT joint_id = 0; joint_id < clip->key_frames.size(); joint_id++)
			{
				auto& frames = clip->key_frames;

				if (frames[joint_id].size() == 0)
				{
					frames[joint_id].resize(clip->end_frame);

					for (UINT frame_idx = 0; frame_idx < frames[joint_id].size(); frame_idx++)
					{
						Joint* joint = joints[joint_id];

						if (joint->parent_id < 0)
						{
							Matrix _mat = Matrix::CreateScale(joint->scale) *
								Matrix::CreateFromQuaternion(joint->rotation) *
								Matrix::CreateTranslation(joint->pos);

							frames[joint_id][frame_idx].mat_joint = _mat;

						}
						else
						{
							Matrix _mat = Matrix::CreateScale(joint->scale) *
								Matrix::CreateFromQuaternion(joint->rotation) *
								Matrix::CreateTranslation(joint->pos);

							frames[joint_id][frame_idx].mat_joint = _mat * frames[joint->parent_id][frame_idx].mat_joint;
						}
					}
				}
				else
				{
					for (UINT frame_idx = 0; frame_idx < frames[joint_id].size(); frame_idx++)
					{
						Joint* joint = joints[joint_id];

						if (joint->parent_id < 0)
						{
							Matrix _mat = Matrix::CreateScale(joint->scale) *
								Matrix::CreateFromQuaternion(frames[joint_id][frame_idx].rotation) *
								Matrix::CreateTranslation(frames[joint_id][frame_idx].pos);//Matrix::CreateTranslation(joint->pos);// *Matrix::CreateTranslation(frames[joint_id][frame_idx].pos);
							// Trans Joint 위치 고정

							frames[joint_id][frame_idx].mat_joint = _mat;

						}
						else
						{
							Matrix _mat = Matrix::CreateScale(joint->scale) *
								Matrix::CreateFromQuaternion(frames[joint_id][frame_idx].rotation) *
								Matrix::CreateTranslation(frames[joint_id][frame_idx].pos);

							if (joint->parent_id == 0)
							{
								frames[joint_id][frame_idx].mat_joint = _mat * joints[0]->mat_joint;
							}
							else
							{
								frames[joint_id][frame_idx].mat_joint = _mat * frames[joint->parent_id][frame_idx].mat_joint;
							}
						}
					}
				}
			}
		}
	}

	void GLTFLoader::CreateAnimation()
	{
		wstring original_folder_name = L"FF7R Resource";
		wstring user_file_path = gltf_path.parent_path().parent_path().wstring();
		user_file_path.replace(user_file_path.find(original_folder_name), original_folder_name.length(), L"User Resource");

		for (auto* clip : anim_clips)
		{
			clip->name = user_file_path + L"\\Animation\\" + clip->name + L".anim";
			Ptr<Animation3D> _animation = ResourceMgr::GetInst()->FindRes<Animation3D>(clip->name);

			if (_animation != nullptr)
			{
				continue;
			}
			_animation = new Animation3D;

			_animation->SetKey(clip->name);
			_animation->SetRelativePath(clip->name);

			_animation->SetClip(clip, joints.size());

			ResourceMgr::GetInst()->AddRes<Animation3D>(_animation->GetKey(), _animation);
			_animation->Save(clip->name);
		}
	}

	void GLTFLoader::LoadMesh()
	{
		for (const auto& mesh : document.meshes.Elements())
		{
			containers.push_back(Container{});
			Container& container = containers[containers.size() - 1];

			if (anim_clips.size() > 0)
			{
				container.is_anim = true;
			}

			container.index.resize(document.materials.Elements().size());

			UINT _subset_idx = 0;

			for (const auto& primitive : mesh.primitives)
			{
				vector<Vtx> _vertex;
				std::string _accessor_id;

				// position 추출
				if (primitive.TryGetAttributeAccessorId(ACCESSOR_POSITION, _accessor_id))
				{
					const Accessor& accessor = document.accessors.Get(_accessor_id);

					const vector<float>& pos_data = resource_reader->ReadBinaryData<float>(document, accessor);

					// 일단 위치 데이터 읽을 때, 정점 버퍼 개수 알아 오는데 그 전에 알아낼 수 있으면 수정
					// 만약 알아낼 방법이 없으면, 임시 변수에 넣었다가 마지막에 취합하는 식으로 수정해야 될듯
					_vertex.resize(pos_data.size() / 3);

					for (int i = 0; i < pos_data.size() / 3; i++)
					{
						vec3 _pos;
						_pos.x = pos_data[i * 3];
						_pos.y = pos_data[i * 3 + 1];
						_pos.z = pos_data[i * 3 + 2];

						_vertex[i].pos = _pos;
					}
				}

				// normal 추출
				if (primitive.TryGetAttributeAccessorId(ACCESSOR_NORMAL, _accessor_id))
				{
					const Accessor& accessor = document.accessors.Get(_accessor_id);
					const vector<float>& normal_data = resource_reader->ReadBinaryData<float>(document, accessor);

					for (int i = 0; i < normal_data.size() / 3; i++)
					{
						vec3 _normal;
						_normal.x = normal_data[i * 3];
						_normal.y = normal_data[i * 3 + 1];
						_normal.z = normal_data[i * 3 + 2];

						_vertex[i].normal = _normal;
						_vertex[i].normal.Normalize();
					}
				}

				// tangent 추출, binormal 계산
				if (primitive.TryGetAttributeAccessorId(ACCESSOR_TANGENT, _accessor_id))
				{
					const Accessor& accessor = document.accessors.Get(_accessor_id);
					const vector<float>& tangent_data = resource_reader->ReadBinaryData<float>(document, accessor);

					for (int i = 0; i < tangent_data.size() / 4; i++)
					{
						vec3 _tangent;
						_tangent.x = tangent_data[i * 4];
						_tangent.y = tangent_data[i * 4 + 1];
						_tangent.z = tangent_data[i * 4 + 2];

						_vertex[i].tangent = _tangent;
						_vertex[i].tangent.Normalize();

						vec3 _binormal;
						_vertex[i].normal.Cross(_vertex[i].tangent, _binormal);
						_vertex[i].binormal = _binormal * tangent_data[i * 4 + 3];
						_vertex[i].binormal.Normalize();
					}
				}

				// uv 추출
				if (primitive.TryGetAttributeAccessorId(ACCESSOR_TEXCOORD_0, _accessor_id))
				{
					const Accessor& accessor = document.accessors.Get(_accessor_id);
					const vector<float>& uv_data = resource_reader->ReadBinaryData<float>(document, accessor);

					for (int i = 0; i < uv_data.size() / 2; i++)
					{
						vec2 _uv;
						_uv.x = uv_data[i * 2];
						_uv.y = uv_data[i * 2 + 1];

						_vertex[i].uv = _uv;
					}
				}

				// 인덱스 추출
				if (!primitive.indicesAccessorId.empty())
				{
					const Accessor& accessor = document.accessors.Get(primitive.indicesAccessorId);

					if (accessor.componentType == COMPONENT_UNSIGNED_INT)
					{
						const vector<uint32_t>& index_data = resource_reader->ReadBinaryData<uint32_t>(document, accessor);

						for (int i = 0; i < index_data.size() / 3; i++)
						{
							container.index[_subset_idx].push_back(static_cast<UINT>(index_data[i * 3]));
							container.index[_subset_idx].push_back(static_cast<UINT>(index_data[i * 3 + 1]));
							container.index[_subset_idx].push_back(static_cast<UINT>(index_data[i * 3 + 2]));
						}
					}
					else
					{
						const vector<uint16_t>& index_data = resource_reader->ReadBinaryData<uint16_t>(document, accessor);

						for (int i = 0; i < index_data.size() / 3; i++)
						{
							container.index[_subset_idx].push_back(static_cast<UINT>(index_data[i * 3]));
							container.index[_subset_idx].push_back(static_cast<UINT>(index_data[i * 3 + 1]));
							container.index[_subset_idx].push_back(static_cast<UINT>(index_data[i * 3 + 2]));
						}
					}
				}

				// 정점 별 조인트 가중치
				if (primitive.TryGetAttributeAccessorId(ACCESSOR_JOINTS_0, _accessor_id))
				{
					string _weight_id;

					if (primitive.TryGetAttributeAccessorId(ACCESSOR_WEIGHTS_0, _weight_id))
					{
						const Accessor& joint_accessor = document.accessors.Get(_accessor_id);
						const Accessor& weight_accessor = document.accessors.Get(_weight_id);

						const vector<uint16_t>& joint_id_data = resource_reader->ReadBinaryData<uint16_t>(document, joint_accessor);
						const vector<uint8_t>& weight_id_data = resource_reader->ReadBinaryData<uint8_t>(document, weight_accessor);

						for (int i = 0; i < joint_id_data.size() / 4; i++)
						{
							uint16_t _joint_id = joint_id_data[i * 4];
							if (_joint_id != 0)
							{
								_vertex[i].indices[0] = _joint_id;
								_vertex[i].weights[0] = weight_id_data[i * 4] / 255.f;

							}
							_joint_id = joint_id_data[i * 4 + 1];
							if (_joint_id != 0)
							{
								_vertex[i].indices[1] = _joint_id;
								_vertex[i].weights[1] = weight_id_data[i * 4 + 1] / 255.f;

							}
							_joint_id = joint_id_data[i * 4 + 2];
							if (_joint_id != 0)
							{
								_vertex[i].indices[2] = _joint_id;
								_vertex[i].weights[2] = weight_id_data[i * 4 + 2] / 255.f;

							}
							_joint_id = joint_id_data[i * 4 + 3];
							if (_joint_id != 0)
							{
								_vertex[i].indices[3] = _joint_id;
								_vertex[i].weights[3] = weight_id_data[i * 4 + 3] / 255.f;

							}
						}
					}
				}

				container.material_id.push_back((UINT)stoi(primitive.materialId));
				container.vertex.push_back(_vertex);
				_subset_idx++;
			}
		}
	}

	void GLTFLoader::LoadMaterial()
	{
		for (UINT i = 0; i < containers.size(); i++)
		{
			// 만약 하나의 gltf파일이 여러개의 mesh가 있을 때 material이 어떻게 들어있는지 확인 안됨.
			// 일단은 하나의 gltf파일에 있으면 material은 하나로 묶이는거 같은데, 만약 하나로 안 묶이거나, 
			// 필요없는 material이 들어가게 되면 코드 수정 (2개의 for문 위치를 변경한 다음 수정 해야 될 듯)
			containers[i].materials.resize(document.materials.Elements().size());
			for (const auto& material : document.materials.Elements())
			{
				GLTFMtrl _mtrl_info;

				_mtrl_info.name = wstring(material.name.begin(), material.name.end());
				_mtrl_info.factor.color_factor = ColorToVec4(material.metallicRoughness.baseColorFactor);
				_mtrl_info.factor.metalic_factor = material.metallicRoughness.metallicFactor;
				_mtrl_info.factor.roughness_factor = material.metallicRoughness.roughnessFactor;
				_mtrl_info.factor.emissive_factor = ColorToVec3(material.emissiveFactor);

				path _parent_path = gltf_path.parent_path().parent_path();
				path _mtrl_path = PathMgr::GetInst()->GetContentPath() + _parent_path.wstring() + L"\\Material\\" + _mtrl_info.name + L".props.txt";
				//wstring _tex_path = _parent_path.wstring() + L"\\Texture\\";

				if (_mtrl_info.name == L"WorldGridMaterial")
				{
					_mtrl_path = wstring(PathMgr::GetInst()->GetContentPath()) + L"FF7R Resource\\Engine\\EngineMaterials\\" + _mtrl_info.name + L".props.txt";
				}

				wstring _content_folder_name = L"FF7R Resource";

				std::smatch _matches;
				//std::regex _tex_pattern(R"(Texture2D'(.+?)')"); // Texture2D 뒤의 문자열 중 ' ' 안의 가장 짧은 문자열 추출
				std::regex _info_pattern(R"(ParameterInfo = \{([^}]+)\})");

				std::ifstream _mtrl_file;
				_mtrl_file.open(_mtrl_path);

				if (_mtrl_file.is_open())
				{
					string _line;
					vector<std::pair<TEX_PARAM, wstring>> _textures;

					while (std::getline(_mtrl_file, _line))
					{
						if (std::regex_search(_line, _matches, _info_pattern))
						{
							string _match = _matches[1].str();

							size_t _pos = _match.rfind(":");

							if (_pos != string::npos)
							{
								string _info = _match.substr(_pos + 1, _match.length() - _pos - 2);
								TEX_PARAM _tex_type = TEX_END;

								if (_info == "Coverage")
								{
									// 용도를 모르겠다
									//_tex_type = TEX_PARAM::COLOR_TEX;
								}
								else if (_info == "Metallic/Roughness/Variant")
								{
									_tex_type = TEX_PARAM::MRV_TEX;
								}
								else if (_info == "Emissive")
								{
									_tex_type = TEX_PARAM::EMISSIVE_TEX;
								}
								else if (_info == "Color")
								{
									_tex_type = TEX_PARAM::COLOR_TEX; 
								}
								else if (_info == "Normal")
								{
									_tex_type = TEX_PARAM::NORMAL_TEX;
								}
								else if (_info == "Occlusion")
								{
									_tex_type = TEX_PARAM::OCCLUSION_TEX;
								}

								std::getline(_mtrl_file, _line);

								std::smatch _tex_match;
								std::regex _tex_pattern(R"(Texture2D'(.+?)')");

								if (std::regex_search(_line, _tex_match, _tex_pattern))
								{
									string _tex_path = _tex_match[1].str();
									size_t _tex_pos = _tex_path.find("Content");

									if (_tex_pos != string::npos)
									{
										_tex_path = _tex_path.substr(_tex_pos + string("Content").length());

										// 확장자 제거하기
										_tex_pos = _tex_path.rfind('.');
										if (_tex_pos != string::npos)
										{
											_tex_path = _tex_path.substr(0, _tex_pos);
											std::replace(_tex_path.begin(), _tex_path.end(), '/', '\\');
											_textures.push_back({ _tex_type, wstring(_tex_path.begin(), _tex_path.end())});
										}

									}

								}

							}

						}

					}
					_mtrl_file.close();

					for (const auto& tex_name : _textures)
					{
						switch (tex_name.first)
						{
						case COLOR_TEX:		_mtrl_info.color = L"" + _content_folder_name + tex_name.second + L".png";		break;
						case NORMAL_TEX:	_mtrl_info.normal = L"" + _content_folder_name + tex_name.second + L".png";		break;
						case MRV_TEX:		_mtrl_info.mrv = L"" + _content_folder_name + tex_name.second + L".png";			break;
						case OCCLUSION_TEX: _mtrl_info.occlusion = L"" + _content_folder_name + tex_name.second + L".png";	break;
						case EMISSIVE_TEX:	_mtrl_info.emissive = L"" + _content_folder_name + tex_name.second + L".hdr";		break;
						default: break;
						}
					}
				}
				else
				{
					// material 파일 못찾음
					// std::cerr << "Unable to find material file" << std::endl;
					assert(nullptr);
				}

				containers[i].materials[stoi(material.id)] = _mtrl_info;
			}
		}
	}

	void GLTFLoader::CreateMaterial()
	{
		wstring original_folder_name = L"FF7R Resource";
		wstring user_file_path = gltf_path.parent_path().parent_path().wstring();
		user_file_path.replace(user_file_path.find(original_folder_name), original_folder_name.length(), L"User Resource");

		for (UINT i = 0; i < containers.size(); i++)
		{
			for (auto& mtrl_info : containers[i].materials)
			{
				mtrl_info.name = user_file_path + L"\\Material\\" + mtrl_info.name + L".mtrl";
				
				Ptr<Material> _material = ResourceMgr::GetInst()->FindRes<Material>(mtrl_info.name);
				if (_material != nullptr)
				{
					continue;
				}

				_material = new Material;

				_material->SetKey(mtrl_info.name);
				_material->SetRelativePath(mtrl_info.name);

				_material->SetShader(ResourceMgr::GetInst()->FindRes<GraphicsShader>(L"DeferredDefault3DShader"));

				wstring _tex_path = mtrl_info.color;
				Ptr<Texture> _tex = ResourceMgr::GetInst()->FindRes<Texture>(_tex_path);
				if (NULL != _tex)
				{
					_material->SetTexParam(TEX_PARAM::COLOR_TEX, _tex);
				}

				_tex_path = mtrl_info.mrv;
				_tex = ResourceMgr::GetInst()->FindRes<Texture>(_tex_path);
				if (NULL != _tex)
				{
					_material->SetTexParam(TEX_PARAM::MRV_TEX, _tex);
				}

				_tex_path = mtrl_info.normal;
				_tex = ResourceMgr::GetInst()->FindRes<Texture>(_tex_path);
				if (NULL != _tex)
				{
					_material->SetTexParam(TEX_PARAM::NORMAL_TEX, _tex);
				}

				_tex_path = mtrl_info.occlusion;
				_tex = ResourceMgr::GetInst()->FindRes<Texture>(_tex_path);
				if (NULL != _tex)
				{
					_material->SetTexParam(TEX_PARAM::OCCLUSION_TEX, _tex);
				}

				_tex_path = mtrl_info.emissive;
				if(!_tex_path.empty())
				{
					_tex = ResourceMgr::GetInst()->FindRes<Texture>(_tex_path);

					if (_tex == nullptr)
					{
						int _pos = _tex_path.rfind('.');
						if (_pos != std::string::npos)
						{
							_tex_path = _tex_path.substr(0, _pos);
							_tex_path += L".png";
						}
						_tex = ResourceMgr::GetInst()->FindRes<Texture>(_tex_path);
					}
					if (NULL != _tex)
					{
						_material->SetTexParam(TEX_PARAM::EMISSIVE_TEX, _tex);
					}
				}


				_material->SetMaterialFactor(mtrl_info.factor);

				ResourceMgr::GetInst()->AddRes<Material>(_material->GetKey(), _material);

				_material->Save(mtrl_info.name);
			}
		}
	}


}