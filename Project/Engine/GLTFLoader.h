#pragma once

#include "global.h"

#include "GLTFSDK/GLBResourceReader.h"
#include "GLTFSDK/GLTFResourceReader.h"
#include "GLTFSDK/Deserialize.h"

#include <iostream>
#include <fstream>
#include <sstream>


namespace ff7r
{
	typedef DirectX::SimpleMath::Quaternion Quater;

	using Microsoft::glTF::InterpolationType;
	using Microsoft::glTF::IStreamReader;
	using Microsoft::glTF::Document;
	using Microsoft::glTF::GLTFResourceReader;

	struct GLTFMtrl
	{
		MtrlFactor	factor;
		wstring     name;
		wstring     color;
		wstring		mrv;
		wstring		normal;
		wstring		emissive; // +ldr emissive 정보도 들어있긴 한데 일단 보류
		wstring		occlusion;
	};

	struct Container
	{
		wstring								name;
			   
		vector<vector<Vtx>>					vertex;
		vector<vector<UINT>>				index;
		vector<UINT>						material_id;

		vector<GLTFMtrl>					materials;

		bool								is_anim;
	};

	struct KeyFrame
	{	
		Matrix				mat_joint;
		Quater				rotation;
		vec3				pos;

		InterpolationType	interpolation_type;

		double				time;
	};	
			
	struct Joint
	{
		wstring			name;
		int				depth;			// 계층구조 깊이
		int				parent_id;		// 부모 Bone 의 인덱스

		vec3			pos;
		Quater			rotation;
		vec3			scale;

		Matrix			mat_inverse;		// Offset 행렬( -> 뿌리 -> Local)
		Matrix			mat_joint;
	};

	struct AnimClip
	{
		wstring		name;
		float		start_time;
		float		end_time;
		UINT		end_frame;

		vector<float>				frame_times;
		vector<vector<KeyFrame>>	key_frames;
		InterpolationType			interpolation_type; // 수정해야된다
	};

	class GLTFLoader
	{
	public:
		GLTFLoader();
		~GLTFLoader();

		void LoadGLTF(const path& _path);

		const Container& GetContainer(UINT _idx = 0) { return containers[_idx]; }

		vector<Joint*>& GetBones() { return joints; }
		vector<AnimClip*>& GetAnimClip() { return anim_clips; }

	private:
		void LoadSkeleton();
		void LoadChlidSkeleton(string _joint_id, string _parent_id, int _depth);
		void LoadAnimation();
		void CalcWorldTransJoint();
		void CreateAnimation();

		void LoadMesh();
		void LoadMaterial();
		void CreateMaterial();

		GLTFResourceReader*				resource_reader;
		Document						document;

		vector<Container>				containers;

		// Animation
		vector<Joint*>					joints;
		vector<AnimClip*>				anim_clips;

		path gltf_path;
	};

	class StreamReader : public IStreamReader
	{
	public:
		StreamReader(path _parent_path) : parent_path(std::move(_parent_path))
		{
			// assert(m_pathBase.has_root_path());
		}

		// Resolves the relative URIs of any external resources declared in the glTF manifest
		std::shared_ptr<std::istream> GetInputStream(const std::string& _file_name) const override
		{
			// In order to construct a valid stream:
			// 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
			//    correctly construct a path instance.
			// 2. Generate an absolute path by concatenating m_pathBase with the specified filename
			//    path. The filesystem::operator/ uses the platform's preferred directory separator
			//    if appropriate.
			// 3. Always open the file stream in binary mode. The glTF SDK will handle any text
			//    encoding issues for us.
			auto streamPath = parent_path / u8path(_file_name);
			auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

			// Check if the stream has no errors and is ready for I/O operations
			if (!stream || !(*stream))
			{
				throw std::runtime_error("Unable to create a valid input stream for uri: " + _file_name);
			}

			return stream;
		}

	private:
		path parent_path;
	};
}

