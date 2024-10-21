#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::wstring;
using std::string;
using std::function;

#include <typeinfo>
#include <assert.h>


// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;



#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h>					// DirectX11
#include <d3dcompiler.h>			// Shader ÄÄÆÄÀÏ
#include <DirectXMath.h>			// DX Math
#include <DirectXPackedVector.h>	

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;

#include "GLTFSDK/GLTF.h"
//#include "GLTFSDK/GLBResourceReader.h"
//#include "GLTFSDK/GLTFResourceReader.h"
//#include "GLTFSDK/Deserialize.h"


#ifdef _DEBUG
#pragma comment(lib, "GLTFSDK/GLTFSDK.lib")
#else
#pragma comment(lib, "GLTFSDK/GLTFSDK.lib")
#endif



#pragma comment(lib, "PhysX/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/PhysX_64.lib")
#pragma comment(lib, "PhysX/PhysXFoundation_64.lib")
#pragma comment(lib, "PhysX/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/PhysXFoundation_64.lib")
#pragma comment(lib, "PhysX/SimulationController_static_64.lib")
#pragma comment(lib, "PhysX/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/PhysXPvdSDK_static_64.lib")



#include "define.h"
#include "struct.h"
#include "func.h"
#include "Singleton.h"
//
//#include "PhysX/PxPhysicsAPI.h"
//
//using namespace physx;
