#include "pch.h"
#include "Animator3DUI.h"
#include "ListUI.h"

#include <Engine\GameObject.h>
#include <Engine\Animator3D.h>

namespace ff7r
{

    /*template <typename T, std::size_t N>
    struct Array
    {
        T data[N];
        const size_t size() const { return N; }

        const T operator [] (size_t index) const { return data[index]; }
        operator T* () {
            T* p = new T[N];
            memcpy(p, data, sizeof(data));
            return p;
        }
    };
    template <typename T, typename ... U> Array(T, U...) -> Array<T, 1 + sizeof...(U)>;*/

    Animator3DUI::Animator3DUI()
        : ComponentUI("##Animator3D", COMPONENT_TYPE::ANIMATOR3D)
        , selected_animator(nullptr)
        , selected_state_name(L"")
        , is_activate_controller(false)
        , selected_transition_idx(-1)
        , controller_info(ControllerInfo())
        , is_state_right_click(false)
        , is_param_right_click(false)
        , animations_list({})
        , selected_animation_idx(-1)
        , selected_event_idx(-1)
        , selected_parameter_name(L"")
        , is_activate_anim_manager(false)
        , info_animation_idx(0)
        , frames_graph_data(nullptr)
        , is_add_parameter(false)
        , is_rename_parameter(false)
        , is_transition_check(false)
        , changed_parameter_name(L"")
    {
        SetName("Animator3D");

        controller_info.fit = GraphEditor::FitOnScreen::Fit_None;
        frames_graph_data = new float* [2];
        frames_graph_data[0] = nullptr;
        frames_graph_data[1] = nullptr;
        text_buf[0] = new char[70];
        text_buf[1] = new char[70];
    }
    Animator3DUI::~Animator3DUI()
    {
        for (int i = 0; i < 2; i++)
        {

            delete frames_graph_data[i];
        }
        delete[] frames_graph_data;
        delete text_buf[0];
        delete text_buf[1];
    }

    void Animator3DUI::ShowAnimationsList()
    {
        // animations_list->SetActive(true);
    }


    int Animator3DUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;

        Animator3D* _animator = GetTarget()->GetAnimator3D();
        if (_animator == nullptr)
        {
            selected_animator = nullptr;
            return FALSE;
        }

        if (selected_animator != _animator)
        {

            map<wstring, Ptr<Animation3D>>::iterator _animations_end = _animator->GetAnimationsEnd();
            map<wstring, Ptr<Animation3D>>::iterator _animations_iter = _animator->GetAnimationsBegin();
            int _animations_count = _animator->GetAnimationsCount();
            animations_list.resize(_animations_count);
            for (int i = 0; i < _animations_count; i++)
            {
                animations_list[i] = _animations_iter->first;
                _animations_iter++;
            }
            frames_list = _animator->GetAnimation(animations_list[info_animation_idx])->GetFrames();

            //int _events_count = 0;
            map<wstring, function<void()>>::iterator _events_begin = _animator->GetAnimationEventBegin();
            map<wstring, function<void()>>::iterator _events_end = _animator->GetAnimationEventEnd();
            events_list.clear();
            for (auto _iter = _events_begin; _iter != _events_end; _iter++)
            {
                events_list.push_back(_iter->first);
            }


            for (int i = 0; i < 2; i++)
            {
                if (frames_graph_data[i] != nullptr)
                {
                    delete frames_graph_data[i];
                    frames_graph_data[i] = nullptr;

                    //frames_graph_data[i] = new float[frames_list.size()];
                }
            }
            frames_graph_data[0] = new float[frames_list.size()];
            frames_graph_data[1] = new float[frames_list.size()];

            for (int i = 0; i < frames_list.size(); i++)
            {
                frames_graph_data[0][i] = frames_list[i];
                frames_graph_data[1][i] = 0.0f;
            }


            selected_animator = _animator;
            selected_state_name = L"";
            selected_transition_idx = -1;
            selected_parameter_name = L"";
            selected_animation_idx = -1;
            selected_event_idx = -1;
        }
        ImGui::Checkbox("Show Animator Controller", &is_activate_controller);
        ImGui::Checkbox("Show Animations Manager", &is_activate_anim_manager);

        if (ImGui::Button("Save##AnimatorSave", ImVec2(200, 18)))
        {
            _animator->SaveAnimator();
        }

        if (is_activate_controller)
        {
            GraphEditorDelegate& _delegate = controller_info.delegate;


            bool showGraphEditor = true;

            map<wstring, AnimationState*>& _anim_states = _animator->GetAnimStates();
            int _states_count = _anim_states.size();
            const char* STR_COMPARISON[(UINT)6] =
            {
                "LESS",
                "GREATER",
                "EQUALS",
                "NOTEQUAL",
                "C_TRUE",
                "C_FLASE",
            };


            //ImGui::Begin("##Selected State");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            if (selected_state_name != L"")
            {
                //wstring _wstr_selected_state_name = selected_state_name;
                string	_str_selected_state_name = string(selected_state_name.begin(), selected_state_name.end());

                //strcpy_s(text_buf, _str_selected_state_name.size() + 1, _str_selected_state_name.c_str());
                ImGui::Separator();
                ImGui::Text("Selected State Info");
                if (!ImGui::InputText("##state_name", text_buf[0], 70))
                {
                    if (!ImGui::IsItemFocused())
                    {
                        string _str_changed_state_name = string(text_buf[0]);
                        wstring _wstr_changed_state_name = wstring(_str_changed_state_name.begin(), _str_changed_state_name.end());

                        if (selected_state_name != _wstr_changed_state_name)
                        {
                            if (_animator->RenameState(selected_state_name, _wstr_changed_state_name))
                            {
                                selected_state_name = _wstr_changed_state_name;
                            }
                            else
                            {
                                strcpy_s(text_buf[0], _str_selected_state_name.size() + 1, _str_selected_state_name.c_str());
                            }
                        }
                    }
                }
                //delete  _text_buf[0];

                AnimationState* _selected_state = _animator->GetAnimState(selected_state_name);

                if (selected_animation_idx == -1)
                {
                    Animation3D* _anim = _selected_state->GetAnimation();
                    if (_anim != nullptr)
                    {
                        wstring _wstr_name = _anim->GetKey();
                        for (int i = 0; i < animations_list.size(); i++)
                        {
                            if (_wstr_name == animations_list[i])
                            {
                                selected_animation_idx = i;
                                break;
                            }
                        }
                    }

                }

                // animation3D UI
                // 
                string _str_param = string("");
                if (selected_animation_idx != -1)
                {
                    wstring _wstr_name = animations_list[selected_animation_idx];
                    //_str_param = string(_wstr_name.begin(), _wstr_name.end());
                    _str_param = path(string(_wstr_name.begin(), _wstr_name.end())).stem().string();
                }
                //wstring _selected_animation_name = animations_list[selected_animation_idx];
                //_temp_trigger.append(string("param_trigger"));
                if (ImGui::BeginCombo("##AnimationsUI", _str_param.c_str()))
                {
                    ImGuiTextFilter _filter;
                    _filter.Draw("##AnimationsFilter");

                    for (int n = 0; n < animations_list.size(); n++)
                    {
                        //string _str_name = string(animations_list[n].begin(), animations_list[n].end());
                        string _str_name = path(string(animations_list[n].begin(), animations_list[n].end())).stem().string();
                        bool is_selected = (n == selected_animation_idx);
                        if (is_selected || !_filter.PassFilter(_str_name.c_str()))
                        {
                            continue;
                        }
                        else if (ImGui::Selectable(_str_name.c_str(), is_selected))
                        {

                            selected_animation_idx = n;
                            _selected_state->SetAnimation(_animator->GetAnimation(animations_list[n]));
                        }
                    }
                    ImGui::EndCombo();
                }

                // transitions UI
                vector<Transition*>& _transitions = _selected_state->GetTransitions();




                int _transitions_count = _transitions.size();
                if (_transitions_count > 0)
                {
                    map<wstring, Parameter*> _parameters = _animator->GetParameters();
                    //const char** _parameter_list = new const char* [_animator->GetParameters().size()];

                    int _params_count = _animator->GetParameters().size();
                    int _comparison_count = 6;
                    map<wstring, Parameter*>::iterator _iter = _parameters.begin();
                    int _param_idx = 0;
                    /*for (; _iter != _parameters.end(); ++_iter)
                    {
                        wstring& _wstr_param_name = _iter->second.GetKey();
                        string _str_param_name = string(_wstr_param_name.begin(), _wstr_param_name.end());
                        char* _param_name = new char[_str_param_name.size() + 1];
                        strcpy_s(_param_name, _str_param_name.size() + 1, _str_param_name.c_str());
                        _parameter_list[_param_idx] = _param_name;
                        _param_idx++;
                    }*/



                    ImGui::Separator();
                    ImGui::Text("Transitions");
                    if (ImGui::BeginListBox("##Transtitions", ImVec2(-FLT_MIN, _transitions_count * 20.0f)))
                    {
                        for (int i = 0; i < _transitions_count; i++)
                        {
                            wstring _output = selected_state_name;
                            _output.append(L" -> ");
                            wstring _input = _transitions[i]->next_state->GetName();
                            _output.append(_input);

                            string _str_transition_name = string(_output.begin(), _output.end());

                            const bool is_selected = (selected_transition_idx == i);

                            if (ImGui::Selectable(_str_transition_name.c_str(), is_selected))
                            {
                                selected_transition_idx = i;

                                Transition* _selected_transition = _transitions[selected_transition_idx];
                                int_buf[0] = _selected_transition->check_frame_idx;
                                int_buf[1] = _selected_transition->start_frame_idx;
                                is_transition_check = _selected_transition->always_chek;
                            }

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndListBox();
                    }
                    if (selected_transition_idx != -1)
                    {
                        Transition* _selected_transition = _transitions[selected_transition_idx];
                        //int _check_frame_idx = _selected_transition->check_frame_idx;
                        //int _start_frame_idx = _selected_transition->start_frame_idx;
                        //ImGui::Text("check frame : "); 
                        //ImGui::SameLine();

                        ImGui::InputInt("check frame : ", &int_buf[0]);
                        if (!ImGui::IsItemFocused())
                        {
                            if (int_buf[0] != _selected_transition->check_frame_idx)
                            {
                                _selected_transition->check_frame_idx = int_buf[0];
                            }
                            else
                            {
                                int_buf[0] = _selected_transition->check_frame_idx;
                            }
                        }
                        ImGui::InputInt("start frame : ", &int_buf[1]);
                        if (!ImGui::IsItemFocused())
                        {
                            if (int_buf[1] != _selected_transition->start_frame_idx)
                            {
                                _selected_transition->start_frame_idx = int_buf[1];
                            }
                            else
                            {
                                int_buf[1] = _selected_transition->start_frame_idx;
                            }
                        }
                        if (ImGui::Checkbox("always check", &is_transition_check))
                        {
                            _selected_transition->always_chek = is_transition_check;
                        }



                        //ImGui::Text("start frame : ");
                        //ImGui::SameLine();
                        ImGui::Text("Conditions");
                        vector<Condition> _conditions = _selected_transition->conditions;
                        int _conditions_count = _conditions.size();




                        if (ImGui::BeginListBox("##Conditions", ImVec2(-FLT_MIN, _conditions_count * 20.0f)))
                        {
                            for (int i = 0; i < _conditions.size(); i++)
                            {
                                wstring _wstr_param = _conditions[i].input_data->GetKey();
                                string _str_param = string(_wstr_param.begin(), _wstr_param.end());

                                int _selected_idx = -1;

                                //ImGui::Text("");
                                //ImGui::SameLine(100.0f, 0.0f);
                                string _temp = string("##");
                                _temp.append(std::to_string(i));

                                switch (_conditions[i].input_data->type)
                                {
                                case DATA_TYPE::INT:
                                {
                                    ImGui::SetNextItemWidth(150);

                                    //if (ImGui::BeginCombo("##a", _str_param.c_str()))
                                    string _temp_int = _temp;
                                    _temp_int.append(string("param_int"));
                                    if (ImGui::BeginCombo(_temp_int.c_str(), _str_param.c_str()))
                                    {
                                        for (auto iter = _parameters.begin(); iter != _parameters.end(); iter++)
                                            //for (int n = 0; n < _params_count; n++)
                                        {
                                            wstring _wstr_temp = iter->first;
                                            string _str_temp = string(_wstr_temp.begin(), _wstr_temp.end());
                                            const bool is_selected = (!strcmp(_str_temp.c_str(), _str_param.c_str()));
                                            if (ImGui::Selectable(_str_temp.c_str(), is_selected))
                                            {
                                                string _str_param = _str_temp.c_str();
                                                wstring _wstr_param = wstring(_str_param.begin(), _str_param.end());
                                                _selected_transition->conditions[i].input_data = _animator->GetParameter(_wstr_param);
                                            }
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                    ImGui::SameLine();
                                    int _compare_data = _conditions[i].compare_data.i;
                                    ImGui::SetNextItemWidth(150);

                                    _temp_int = _temp;
                                    _temp_int.append(string("compare_int"));
                                    if (ImGui::InputInt(_temp_int.c_str(), &_compare_data))
                                    {

                                        _selected_transition->conditions[i].compare_data.i = _compare_data;
                                    }

                                    ImGui::SameLine();
                                    ImGui::SetNextItemWidth(150);
                                    int _comparison_idx = (int)_conditions[i].comparsion;

                                    _temp_int = _temp;
                                    _temp_int.append(string("comparison_int"));
                                    if (ImGui::BeginCombo(_temp_int.c_str(), STR_COMPARISON[_comparison_idx]))
                                    {
                                        for (int n = 0; n < _comparison_count; n++)
                                        {
                                            const bool is_selected = (_comparison_idx == n);
                                            if (ImGui::Selectable(STR_COMPARISON[n], is_selected))
                                            {
                                                _selected_transition->conditions[i].comparsion = (COMPARISON)n;
                                            }
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                    break;
                                }
                                case DATA_TYPE::FLOAT:
                                {
                                    ImGui::SetNextItemWidth(150);
                                    string _temp_float = _temp;
                                    _temp_float.append(string("param_FLOAT"));
                                    if (ImGui::BeginCombo(_temp_float.c_str(), _str_param.c_str()))
                                    {
                                        for (auto iter = _parameters.begin(); iter != _parameters.end(); iter++)
                                            //for (int n = 0; n < _params_count; n++)
                                        {
                                            wstring _wstr_temp = iter->first;
                                            string _str_temp = string(_wstr_temp.begin(), _wstr_temp.end());
                                            const bool is_selected = (!strcmp(_str_temp.c_str(), _str_param.c_str()));
                                            if (ImGui::Selectable(_str_temp.c_str(), is_selected))
                                            {
                                                string _str_param = _str_temp.c_str();
                                                wstring _wstr_param = wstring(_str_param.begin(), _str_param.end());
                                                _selected_transition->conditions[i].input_data = _animator->GetParameter(_wstr_param);
                                            }
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                    ImGui::SameLine();
                                    float _compare_data = _conditions[i].compare_data.f;
                                    ImGui::SetNextItemWidth(150);

                                    _temp_float = _temp;
                                    _temp_float.append(string("compare_float"));
                                    if (ImGui::InputFloat(_temp_float.c_str(), &_compare_data))
                                    {

                                        _selected_transition->conditions[i].compare_data.f = _compare_data;
                                    }
                                    int _comparison_idx = (int)_conditions[i].comparsion;

                                    ImGui::SameLine();
                                    ImGui::SetNextItemWidth(150);
                                    _temp_float = _temp;
                                    _temp_float.append(string("comparison_float"));
                                    if (ImGui::BeginCombo(_temp_float.c_str(), STR_COMPARISON[_comparison_idx]))
                                    {
                                        for (int n = 0; n < _comparison_count; n++)
                                        {
                                            const bool is_selected = (_comparison_idx == n);
                                            if (ImGui::Selectable(STR_COMPARISON[n], is_selected))
                                            {
                                                _selected_transition->conditions[i].comparsion = (COMPARISON)n;
                                            }
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                    break;
                                }
                                case DATA_TYPE::BOOL:
                                {
                                    ImGui::SetNextItemWidth(150);
                                    string _temp_bool = _temp;
                                    _temp_bool.append(string("param_bool"));
                                    if (ImGui::BeginCombo(_temp_bool.c_str(), _str_param.c_str()))
                                    {
                                        for (auto iter = _parameters.begin(); iter != _parameters.end(); iter++)
                                            //for (int n = 0; n < _params_count; n++)
                                        {
                                            wstring _wstr_temp = iter->first;
                                            string _str_temp = string(_wstr_temp.begin(), _wstr_temp.end());
                                            const bool is_selected = (!strcmp(_str_temp.c_str(), _str_param.c_str()));
                                            if (ImGui::Selectable(_str_temp.c_str(), is_selected))
                                            {
                                                string _str_param = _str_temp.c_str();
                                                wstring _wstr_param = wstring(_str_param.begin(), _str_param.end());
                                                _selected_transition->conditions[i].input_data = _animator->GetParameter(_wstr_param);
                                            }
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                    ImGui::SameLine();
                                    ImGui::SetNextItemWidth(150);

                                    const char* _bool_list[2] = { "true", "false" };
                                    int _comparison_idx = (_conditions[i].comparsion == COMPARISON::C_TRUE) ? 0 : 1;

                                    _temp_bool = _temp;
                                    _temp_bool.append(string("comparison_bool"));
                                    if (ImGui::BeginCombo(_temp_bool.c_str(), _bool_list[_comparison_idx]))
                                    {
                                        for (int n = 0; n < 2; n++)
                                        {
                                            const bool is_selected = (_comparison_idx == n);
                                            if (ImGui::Selectable(_bool_list[n], is_selected))
                                            {
                                                _selected_transition->conditions[i].comparsion = (COMPARISON)(n + 4);
                                            }
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                    break;
                                }
                                case DATA_TYPE::TRIGGER:
                                {
                                    ImGui::SetNextItemWidth(150);

                                    string _temp_trigger = _temp;
                                    _temp_trigger.append(string("param_trigger"));
                                    if (ImGui::BeginCombo(_temp_trigger.c_str(), _str_param.c_str()))
                                    {
                                        for (auto iter = _parameters.begin(); iter != _parameters.end(); iter++)
                                            //for (int n = 0; n < _params_count; n++)
                                        {
                                            wstring _wstr_temp = iter->first;
                                            string _str_temp = string(_wstr_temp.begin(), _wstr_temp.end());
                                            const bool is_selected = (!strcmp(_str_temp.c_str(), _str_param.c_str()));
                                            if (ImGui::Selectable(_str_temp.c_str(), is_selected))
                                            {
                                                string _str_param = _str_temp.c_str();
                                                wstring _wstr_param = wstring(_str_param.begin(), _str_param.end());
                                                _selected_transition->conditions[i].input_data = _animator->GetParameter(_wstr_param);
                                            }
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                    break;
                                }
                                default:
                                    break;

                                }

                            }


                            ImGui::EndListBox();
                            if (ImGui::Button("+", ImVec2(20.0f, 20.0f)))
                            {

                                //string _str_parameter = string();
                                //wstring _wstr_parameter = wstring(_str_parameter.begin(), _str_parameter.end());
                                wstring _wstr_parameter = _animator->GetParameters().begin()->first;
                                _selected_state->AddCondition(_selected_transition, _wstr_parameter, 0, COMPARISON::EQUALS);
                            }
                        }

                    }


                    /*for (int i = 0; i < _animator->GetParameters().size(); i++)
                    {
                        delete _parameter_list[i];
                    }
                    delete[] _parameter_list;*/
                }
            }
            //ImGui::End();


            _delegate.mNodes.clear();
            _delegate.mTemplates.clear();
            _delegate.mLinks.clear();

            vector<vector<int>> _input;
            _input.resize(_states_count);
            vector<vector<int>> _output;
            _output.resize(_states_count);

            int _state_idx = 0;
            map<wstring, AnimationState*>::iterator iter = _anim_states.begin();
            for (; iter != _anim_states.end(); ++iter)
            {
                wstring _temp = iter->first;
                AnimationState* _state = _anim_states[_temp];
                vec2 _node_pos = _state->GetNodePosition();

                wstring _wstr_state_name = _state->GetName();
                string _str_state_name = string(_wstr_state_name.begin(), _wstr_state_name.end());
                char* _state_name = new char[_str_state_name.size() + 1];
                strcpy_s(_state_name, _str_state_name.size() + 1, const_cast<char*>(_str_state_name.c_str()));

                GraphEditorDelegate::Node _state_node = {
                    _state_name,
                    _state_idx,
                    _node_pos.x,_node_pos.y,
                    false
                };
                if (selected_state_name == _wstr_state_name)
                {
                    _state_node.mSelected = true;
                }
                _delegate.mNodes.push_back(_state_node);
                _state_idx++;
            }

            int _cur_state_idx = -1;
            int _default_state_idx = -1;
            int _any_state_idx = -1;

            _state_idx = 0;
            iter = _anim_states.begin();
            for (; iter != _anim_states.end(); ++iter)
            {
                //wstring _temp = std::to_wstring(i);
                AnimationState* _state = iter->second;

                if (_animator->GetCurState() == _state)
                {
                    _cur_state_idx = _state_idx;
                }
                else if (_state->GetName() == L"default")
                {
                    _default_state_idx = _state_idx;
                }
                else if (_state->GetName() == L"any_state")
                {
                    _any_state_idx = _state_idx;
                }

                vector<Transition*> _transitions = _state->GetTransitions();
                for (int i = 0; i < _transitions.size(); i++)
                {
                    AnimationState* _next_state = _transitions[i]->next_state;
                    if (_next_state != nullptr)
                    {
                        for (int j = 0; j < _states_count; j++)
                        {
                            wstring _wstr_next_state = _next_state->GetName();
                            string _str_next_state = string(_wstr_next_state.begin(), _wstr_next_state.end());
                            if (string(_delegate.mNodes[j].name) == _str_next_state)
                            {
                                _delegate.mLinks.push_back(GraphEditor::Link{ (size_t)_state_idx, _output[_state_idx].size(),  _delegate.mNodes[j].templateIndex, _input[_delegate.mNodes[j].templateIndex].size() });
                                _input[_delegate.mNodes[j].templateIndex].push_back(_state_idx);
                                _output[_state_idx].push_back(j);
                                break;
                            }
                        }
                    }
                }
                _state_idx++;
            }

            int _link_count = _delegate.mLinks.size();
            for (int i = 0; i < _states_count; i++)
            {
                int _input_count = _input[i].size();
                int _output_count = _output[i].size();
                const char** _input_names = new const char* [_input_count + 1];
                const char** _output_names = new const char* [_output_count + 1];


                for (int j = 0; j < _input_count; j++)
                {
                    string _str_input_name = _delegate.mNodes[_input[i][j]].name;
                    char* _input_name = new char[_str_input_name.size() + 1];
                    strcpy_s(_input_name, _str_input_name.size() + 1, _str_input_name.c_str());
                    _input_names[j] = _input_name;
                }
                string _str_input_name = string("");
                char* _input_name = new char[_str_input_name.size() + 1];
                strcpy_s(_input_name, _str_input_name.size() + 1, _str_input_name.c_str());
                _input_names[_input_count] = _input_name;
                for (int j = 0; j < _output_count; j++)
                {
                    string _str_output_name = _delegate.mNodes[_output[i][j]].name;
                    char* _output_name = new char[_str_output_name.size() + 1];
                    strcpy_s(_output_name, _str_output_name.size() + 1, const_cast<char*>(_str_output_name.c_str()));
                    _output_names[j] = _output_name;
                }
                string _str_output_name = string("");
                char* _output_name = new char[_str_output_name.size() + 1];
                strcpy_s(_output_name, _str_output_name.size() + 1, _str_output_name.c_str());
                _output_names[_output_count] = _output_name;

                GraphEditor::Template _template = {

                    IM_COL32(160, 160, 180, 255),
                    IM_COL32(100, 100, 140, 255),
                    IM_COL32(110, 110, 150, 255),
                    _input[i].size() + 1,
                    _input_names,
                    nullptr,
                    _output[i].size() + 1,
                    _output_names,
                    nullptr
                };


                if (_cur_state_idx == i)
                {
                    _template.mBackgroundColor = IM_COL32(255, 127, 0, 255);
                }
                else if (_default_state_idx == i)
                {
                    _template.mBackgroundColor = IM_COL32(0, 255, 0, 255);
                }
                else if (_any_state_idx == i)
                {
                    _template.mBackgroundColor = IM_COL32(100, 200, 255, 255);
                }

                _delegate.mTemplates.push_back(_template);

            }
            if (showGraphEditor)
            {
                ImGui::Begin("##Parameters", NULL, 0);
                {
                    ImGuiTextFilter _filter;
                    bool _is_window_focused = ImGui::IsWindowFocused();
                    map<wstring, Parameter*>& _parameters = _animator->GetParameters();
                    _filter.Draw("##ParametersFilter");
                    ImGui::SameLine();


                    if (is_add_parameter)
                    {
                        ImGui::OpenPopup("##AddParameterPopup");
                        if (ImGui::BeginPopup("##AddParameterPopup"))
                        {
                            const char* _parameter_types[4] = { "INT","FLOAT", "BOOL", "TRIGGER" };
                            for (int j = 0; j < 4; j++)
                            {
                                if (ImGui::Selectable(_parameter_types[j]))
                                {
                                    string _key = string("New ");
                                    _key.append(string(_parameter_types[j]));
                                    wstring _wstr_key = wstring(_key.begin(), _key.end());
                                    int _temp = 0;
                                    wstring _wstr_add_key = _wstr_key;
                                    while (_parameters.find(_wstr_add_key) != _parameters.end())
                                    {
                                        _wstr_add_key = _wstr_key;
                                        _temp++;
                                        _wstr_add_key.append(std::to_wstring(_temp));
                                    }
                                    _wstr_key = _wstr_add_key;
                                    /*if (_temp > 0)
                                    {
                                         = std::to_wstring(_temp);
                                        _wstr_key.append(_wstr_add_key);
                                    }*/
                                    Parameter* _temp_param = new Parameter;
                                    _temp_param->data = false;
                                    _temp_param->type = DATA_TYPE(j);
                                    _animator->CreateParameter(_wstr_key, _temp_param);
                                    break;
                                }
                            }
                            ImGui::EndPopup();
                        }

                    }
                    if (ImGui::Button("+##AddParameter"))
                    {
                        is_add_parameter = true;
                    }
                    else if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
                    {

                        is_add_parameter = false;
                    }

                    bool _rename_parameter = false;
                    for (auto iter = _parameters.begin(); iter != _parameters.end(); iter++)
                    {

                        wstring _wstr_param = iter->first;
                        string _str_param = string(_wstr_param.begin(), _wstr_param.end());
                        bool is_selected = (_wstr_param == selected_parameter_name);
                        if (!_filter.PassFilter(_str_param.c_str()))
                        {
                            if (is_selected)
                                selected_parameter_name = L"";
                            continue;
                        }
                        /*if (ImGui::Selectable(_str_param.c_str(), is_selected))
                        {
                            string _str_changed_param_name = string(text_buf[1]);
                            wstring _wstr_changed_param_name = wstring(_str_changed_param_name.begin(), _str_changed_param_name.end());
                            if (_wstr_changed_param_name != selected_parameter_name)
                            {
                                _animator->RenameParameter(selected_parameter_name, _wstr_changed_param_name);
                            }
                            selected_parameter_name = _wstr_param;
                            strcpy_s(text_buf[1], _str_param.size() + 1, _str_param.c_str());
                        }*/
                        if (is_selected)
                        {
                            if (is_rename_parameter)
                            {
                                if (!ImGui::InputText("##parameter_name", text_buf[1], 70))
                                {
                                    //ImGuiInputTextFlags
                                    if (!ImGui::IsItemFocused())
                                    {
                                        string _str_changed_param_name = string(text_buf[1]);
                                        wstring _wstr_changed_param_name = wstring(_str_changed_param_name.begin(), _str_changed_param_name.end());

                                        if (selected_parameter_name != _wstr_changed_param_name)
                                        {
                                            _rename_parameter = true;
                                            changed_parameter_name = selected_parameter_name;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (ImGui::Selectable(_str_param.c_str(), is_selected))
                                {
                                    is_rename_parameter = true;
                                    strcpy_s(text_buf[1], _str_param.size() + 1, _str_param.c_str());
                                }
                                else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
                                {
                                    is_param_right_click = true;
                                }
                            }
                        }
                        else if (ImGui::Selectable(_str_param.c_str(), is_selected))
                        {
                            selected_parameter_name = _wstr_param;
                            is_rename_parameter = false;
                        }
                        ImGui::Separator();
                    }
                    if (_rename_parameter)
                    {
                        string _str_changed_param_name = string(text_buf[1]);
                        wstring _wstr_changed_param_name = wstring(_str_changed_param_name.begin(), _str_changed_param_name.end());

                        if (_animator->RenameParameter(changed_parameter_name, _wstr_changed_param_name))
                        {
                            if (changed_parameter_name == selected_parameter_name)
                                selected_parameter_name = _wstr_changed_param_name;

                        }
                        is_rename_parameter = false;
                        /*else
                        {
                            strcpy_s(text_buf[1], _str_param.size() + 1, _str_param.c_str());
                        }*/
                    }
                    else if (is_param_right_click)
                    {
                        ImGui::OpenPopup("##ParameterPopup");
                        if (ImGui::BeginPopup("##ParameterPopup"))
                        {
                            if (ImGui::Selectable("Delete Param"))
                            {
                                selected_animator->DeleteParameter(selected_parameter_name);
                                selected_parameter_name = L"";
                            }
                            if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)
                                || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right)))
                            {
                                is_param_right_click = false;
                            }
                            ImGui::EndPopup();
                        }
                    }
                }
                ImGui::End();

                ImGui::Begin("##State Machine", NULL, 0);
                if (ImGui::Button("Fit all nodes"))
                {
                    controller_info.fit = GraphEditor::Fit_AllNodes;
                }
                ImGui::SameLine();
                if (ImGui::Button("Fit selected nodes"))
                {
                    controller_info.fit = GraphEditor::Fit_SelectedNodes;
                }
                GraphEditor::Show(_delegate, controller_info.options, controller_info.viewState, true, &controller_info.fit);
                //bool is_window_focused = ImGui::IsWindowFocused()
                if (is_state_right_click)// && ImGui::IsWindowFocused())
                {

                    ImGui::OpenPopup("##AnimatorControllerPopup");
                    if (ImGui::BeginPopup("##AnimatorControllerPopup"))
                    {
                        if (is_item_right_click)
                        {
                            if (ImGui::Selectable("Delete State"))
                            {
                                selected_animator->DeleteState(selected_state_name);
                                selected_state_name = L"";
                            }
                            if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)
                                || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right)))
                            {
                                is_item_right_click = false;
                                is_state_right_click = false;
                            }
                        }
                        else
                        {
                            if (ImGui::Selectable("Create State"))
                            {
                                selected_animator->CreateEmptyState(L"");
                            }
                            if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)
                                || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right)))
                            {
                                is_state_right_click = false;
                            }
                        }
                        ImGui::EndPopup();
                    }

                }
                else if (ImGui::IsItemHovered())
                {
                    if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
                    {
                        is_state_right_click = true;
                        if (selected_state_hovered)
                        {
                            is_item_right_click = true;
                        }
                    }
                }
                ImGui::End();
            }

            wstring _new_transition_input = L"";
            int _new_link_idx = -1;
            if (_link_count < _delegate.mLinks.size())
            {
                string _name = _delegate.mNodes[_delegate.mLinks[_delegate.mLinks.size() - 1].mInputNodeIndex].name;
                _new_transition_input = wstring(_name.begin(), _name.end());
                _new_link_idx = _delegate.mLinks.size() - 1;
            }

            for (int i = 0; i < _delegate.mNodes.size(); i++)
            {
                GraphEditorDelegate::Node _temp_node = _delegate.mNodes[i];
                string _name = _temp_node.name;
                //wstring _temp = std::to_wstring(i);
                wstring _temp = wstring(_name.begin(), _name.end());
                if (_anim_states.find(_temp) == _anim_states.end())
                {
                    continue;
                }
                else
                {
                    AnimationState* _state = _anim_states[_temp];
                    _state->SetNodePosition(vec2(_temp_node.x, _temp_node.y));
                    if (_temp_node.mSelected)
                    {
                        if (_temp != selected_state_name)
                        {
                            selected_state_name = _temp;
                            selected_animation_idx = -1;
                            selected_transition_idx = -1;

                            string _str_selected_state_name = string(selected_state_name.begin(), selected_state_name.end());
                            strcpy_s(text_buf[0], _str_selected_state_name.size() + 1, _str_selected_state_name.c_str());
                        }
                        else
                        {
                            if (_delegate.hovered_node_idx == i)
                                selected_state_hovered = true;
                            else
                                selected_state_hovered = false;
                        }
                    }
                    //if (_temp == _new_transition_input)
                    if (_temp == _new_transition_input)
                    {
                        int _output_idx = _delegate.mLinks[_new_link_idx].mOutputNodeIndex;
                        if (_output_idx != i)
                        {
                            string _name = _delegate.mNodes[_delegate.mLinks[_new_link_idx].mOutputNodeIndex].name;
                            wstring _new_transition_output = wstring(_name.begin(), _name.end());
                            vector<Transition*> _transitions = _state->GetTransitions();
                            bool _is_new_transition = true;
                            for (int j = 0; j < _transitions.size(); j++)
                            {
                                if (_transitions[j]->next_state->GetName() == _new_transition_output)
                                {
                                    _is_new_transition = false;
                                    break;
                                }
                            }
                            if (_is_new_transition)
                                _animator->CreateTransition(_anim_states[_new_transition_input], _anim_states[_new_transition_output]);
                        }

                    }
                }
            }
            for (int i = 0; i < _delegate.mNodes.size(); i++)
            {
                delete _delegate.mNodes[i].name;
            }
            for (int i = 0; i < _delegate.mTemplates.size(); i++)
            {

                for (int j = 0; j < _delegate.mTemplates[i].mInputCount; j++)
                {
                    delete _delegate.mTemplates[i].mInputNames[j];
                }
                delete[] _delegate.mTemplates[i].mInputNames;
                for (int j = 0; j < _delegate.mTemplates[i].mOutputCount; j++)
                {
                    delete _delegate.mTemplates[i].mOutputNames[j];
                }
                delete[] _delegate.mTemplates[i].mOutputNames;
            }
        }


        if (is_activate_anim_manager)
        {
            ImGui::Begin("Animation", NULL, 0);

            ImGui::Columns(2, "col");

            wstring _wstr_param = animations_list[info_animation_idx];
            //string _str_param = string(_wstr_param.begin(), _wstr_param.end());
            string _str_param = path(string(_wstr_param.begin(), _wstr_param.end())).stem().string();
            if (ImGui::BeginCombo("##AnimationsInfoUI", _str_param.c_str()))
            {
                for (int n = 0; n < animations_list.size(); n++)
                {
                   // string _str_name = string(animations_list[n].begin(), animations_list[n].end());
                    string _str_name = path(string(animations_list[n].begin(), animations_list[n].end())).stem().string();
                    bool is_selected = (n == info_animation_idx);
                    if (is_selected)
                    {
                        continue;
                    }
                    if (ImGui::Selectable(_str_name.c_str(), is_selected))
                    {
                        info_animation_idx = n;
                        frames_list = _animator->GetAnimation(animations_list[info_animation_idx])->GetFrames();

                        for (int i = 0; i < 2; i++)
                        {
                            if (frames_graph_data[i] != nullptr)
                            {
                                delete frames_graph_data[i];
                                frames_graph_data[i] = nullptr;
                            }
                        }
                        frames_graph_data[0] = new float[frames_list.size()];
                        frames_graph_data[1] = new float[frames_list.size()];
                        for (int i = 0; i < frames_list.size(); i++)
                        {
                            frames_graph_data[0][i] = frames_list[i];
                            frames_graph_data[1][i] = 0.0f;
                        }
                    }
                }


                ImGui::EndCombo();
            }
            Animation3D* _selected_animation = _animator->GetAnimation(_wstr_param).Get();
            vector<std::pair<int, wstring>> _anim_events = _selected_animation->GetAnimationEvents();
            int _events_count = _anim_events.size();

            if (ImGui::BeginListBox("##AnimEvents", ImVec2(-FLT_MIN, _events_count * 30.0f)))
            {
                for (int i = 0; i < _events_count; i++)
                {
                    wstring _wstr_event = _anim_events[i].second;
                    string _str_event = string(_wstr_event.begin(), _wstr_event.end());


                    //ImGui::SameLine(100.0f, 0.0f);

                    int _event_frame = _anim_events[i].first;

                    bool is_selected = (i == selected_event_idx);



                    //ImGui::SameLine();
                    string _str_temp = "##";
                    _str_temp.append(std::to_string(i));
                    if (is_selected)
                    {
                        ImGui::SetNextItemWidth(150);
                        string _str_event_frame = _str_temp;
                        _str_event_frame.append(string("event_frame"));
                        ImGui::InputInt(_str_event_frame.c_str(), &int_buf[2]);
                        if (!ImGui::IsItemFocused())
                        {
                            if (int_buf[2] != _anim_events[i].first)
                            {
                                if (!_selected_animation->UpdateAnimationEvent(i, _anim_events[i].second, int_buf[2]))
                                {
                                    int_buf[2] = _anim_events[i].first;
                                }
                            }
                        }
                    }
                    else
                    {
                        string _str_event_frame = _str_temp;
                        _str_event_frame.append(string("event_frame"));
                        if (ImGui::Selectable(_str_event_frame.c_str(), is_selected))
                        {
                            int_buf[2] = _anim_events[i].first;
                            selected_event_idx = i;
                        }
                        ImGui::SameLine();
                        ImGui::SetNextItemWidth(150);

                        string _temp = std::to_string(_anim_events[i].first);
                        ImGui::Text(_temp.c_str());
                    }

                    ImGui::SameLine();

                    string _str_event_name = _str_temp;
                    _str_event_name.append(string("event_name"));
                    if (ImGui::BeginCombo(_str_event_name.c_str(), _str_event.c_str()))
                    {
                        ImGuiTextFilter _filter;
                        _filter.Draw("##EventFilter");

                        for (int n = 0; n < events_list.size(); n++)
                        {
                            wstring _wstr_name = events_list[n];
                            string _str_name = string(_wstr_name.begin(), _wstr_name.end());
                            bool _is_selected = (events_list[n] == _wstr_event);
                            if (_is_selected || !_filter.PassFilter(_str_name.c_str()))
                            {
                                continue;
                            }
                            else if (ImGui::Selectable(_str_name.c_str(), _is_selected))
                            {
                                _selected_animation->UpdateAnimationEvent(i, _wstr_name, _anim_events[i].first);
                            }
                        }
                        ImGui::EndCombo();

                    }
                    ImGui::SameLine();

                    string _str_event_unset = string("-");
                    _str_event_unset.append(_str_temp);
                    if (ImGui::Button(_str_event_unset.c_str(), ImVec2(20.0f, 20.0f)))
                    {
                        _selected_animation->UnsetAnimationEvent(i);
                        selected_event_idx = -1;
                    }
                }
                ImGui::EndListBox();
                if (ImGui::Button("+##EventSet", ImVec2(20.0f, 20.0f)))
                {
                    _selected_animation->SetAnimationEvent(events_list[0]);
                }
            }
            ImGui::NextColumn();

            ImGuiViewport* _viewport = ImGui::GetWindowViewport();
            if (ImPlot::BeginPlot("##plot", ImVec2(_viewport->WorkSize.x / 2.0f, _viewport->WorkSize.y - 20.0f), ImPlotFlags_NoLegend | ImPlotFlags_NoTitle))
            {

                ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_Lock);
                ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_LockMin);
                //ImPlot::PlotScatterG()
                //ImPlot::PlotStems("hiefafe",  idx, 3, 0, 1, 0, ImPlotStemsFlags_Horizontal);
                    //ImPlotScatterFlags flags = 0, int offset = 0, int stride = sizeof(T)
                    //ImPlot::SetupAxis
                ImPlot::SetupAxisLimits(ImAxis_Y1, -0.1f, 0.1f, ImPlotCond_Once);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0.0f, 1.0f, ImPlotCond_Once);
                ImPlot::PlotScatter("##plotscatter", frames_graph_data[0], frames_graph_data[1], frames_list.size());

                ImPlot::EndPlot();
            }
            //ImGui::EndChild();
            ImGui::End();

        }
        return TRUE;
    }
}