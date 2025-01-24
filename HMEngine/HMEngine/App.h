#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <functional>
#include <unordered_map>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "ImGuiUtil.h"
#include <map>
#include <algorithm>

#include "Util.h"
#include "GraphicsCommon.h"
#include "D3DUtil.h"
#include "MeshFactory.h"
#include "Camera.h"
#include "Material.h"
#include "GameObject.h"
#include "Light.h"
#include "CubeMapping.h"
#include "ImageFilter.h"

using namespace std;
using Microsoft::WRL::ComPtr;
using std::wstring;
using namespace Graphics;

extern const float PI;
extern const float ToDegree;
extern const float ToRadian;
extern const bool EditMode;

enum E_SelectedKind
{
	SelectedKind_None,
	SelectedKind_GameObject,
	SelectedKind_Light
};
class App
{
public:
	static App* pIns;
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool Init();
	int Run();
	GameObject* CreateObj(const string& name, const string& modelName, GraphicsPSO* pPSO);
	GameObject* GetObj(const string& name);
	void CreateLight(Light* pLight);

	Model* GetModel(const string& name);
	ComPtr<ID3D11ShaderResourceView> GetTexView(const string& name);
	void LoadTexture(const string& key, const string& filename, const bool useSRGB, const UINT mipLevel);
	void LoadModel(const string& modelName, MeshData meshData);
	const bool isKeyDown(UINT key)const;
	const bool isKeyUp(UINT key)const;
	const bool isKey(UINT key)const;
	const Vector2 GetMouseMove() const;
	const bool isMouseDown(UINT button)const;
	const bool isMouseUp(UINT button)const;
	const bool isMouse(UINT button)const;
private:

	void Start();
	void UpdateGUI();
	void Update(const float deltaTime);
	void Render();

	bool InitGUI();
	bool InitDirect3D();
	bool InitWindow();

	UINT numQualityLevels = 0;
	HWND m_HWND;
	int m_ScreenWidth = 800;
	int m_ScreenHeight = 600;

	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_Context;
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11RenderTargetView> m_MainRTV;
	ComPtr<ID3D11ShaderResourceView> m_MainSRV;
	ComPtr<ID3D11DepthStencilView> m_MainDSV;
	D3D11_VIEWPORT m_ScreenViewport;
	
	ComPtr<ID3D11Texture2D> m_MousePickingTex;
	ComPtr<ID3D11RenderTargetView> m_MousePickingRTV;
	ComPtr<ID3D11Texture2D> m_MousePickingStagingTex;

	int m_MouseCursorColorValue = -1; //커서가 있는곳의 mousepicking rtv 의 컬러값 - 1
	int m_SelectedIdx = 0;
	E_SelectedKind m_SelectedKind = SelectedKind_None;

	unordered_map<size_t, unique_ptr<Model>> m_Models;
	unordered_map<size_t, ComPtr<ID3D11ShaderResourceView>> m_TexViews;
	vector<shared_ptr<GameObject>> m_Objs;
	vector<shared_ptr<Light>> m_Lights;

	Camera* m_Cam;
	GameObject* m_Skybox;
	ComPtr<ID3D11ShaderResourceView> m_EnvIBLSRV;
	ComPtr<ID3D11ShaderResourceView> m_DiffuseIBLSRV;
	ComPtr<ID3D11ShaderResourceView> m_SpecularIBLSRV;
	ComPtr<ID3D11ShaderResourceView> m_BRDFIBLSRV;
	GlobalCBuffer m_GlobalCBufferCPU;
	ComPtr<ID3D11Buffer> m_GlobalCBufferGPU;
	LightCBuffer m_LightCBufferCPU;
	ComPtr<ID3D11Buffer> m_LightCBufferGPU;


	bool m_KeyDown[256] = { false };
	bool m_KeyDown_LastFrame[256] = { false };
	bool m_MouseDown[2] = { false };
	bool m_MouseDown_LastFrame[2] = { false };

	Vector2 m_MousePosNS;
	Vector2 m_MousePosNS_LastFrame;
	Vector2 m_ImGuiLTopNS;
	Vector2 m_ImGuiRBotNS;
	bool m_MouseInGui;
	bool m_MouseisDrag;

	bool m_MoveMode;

};


//Key link : https://learn.microsoft.com/ko-kr/windows/win32/inputdev/virtual-key-codes