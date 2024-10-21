#include "pch.h"

#include "ImageUIScript.h"
namespace ff7r
{
	ImageUIScript::ImageUIScript()
		: UIScript(SCRIPT_TYPE::IMAGEUISCRIPT)
		, type(ImageEffectType::NONE)
		, time_speed(2.0f)
		, time(0.0f)
		, alpha_factor(1.0f)
	{
	}

	ImageUIScript::~ImageUIScript()
	{
	}

	void ImageUIScript::Awake()
	{
		UIScript::Awake();
		GetMeshRender()->GetDynamicMaterial();
	}

	void ImageUIScript::Tick()
	{
		time += DT * time_speed;

		switch (type)
		{
		case ImageEffectType::ALPHA_CHANGE:
		{
			float _alpha = std::fmodf(time, 2.0f);
			_alpha = _alpha > 1.0f ? 2.0f - _alpha : _alpha;
			_alpha *= alpha_factor;

			GetMeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &_alpha);
		}
		break;

		default: break;
		}
	}

	void ImageUIScript::OnEnable()
	{
		time = 0.0f;
	}
}