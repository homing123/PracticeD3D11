
#include "App.h"
const float PI = 3.14159265f;
const float ToDegree = 180 / PI;
const float ToRadian = PI / 180;
const bool EditMode = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


App* App::pIns = nullptr;

void App::Start()
{

	//texture
	ComPtr<ID3D11Texture2D> tex;
	ComPtr<ID3D11ShaderResourceView> srv;
	LoadTexture("../Resource/Tex/UVTex.png", false, tex, srv, 1);
	LoadModel("Sphere", MeshFactory::CreateSphere(1, 100, 100));
	//CreateObj("IBL_Sphere", "Sphere", &IBLPSO);
	CreateObj("BlinnPhong_Sphere_0", "Sphere", &BlinnPhongPSO_0);

	//skybox
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleEnvHDR.dds", m_EnvIBLSRV, true);
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleDiffuseHDR.dds", m_DiffuseIBLSRV, true);
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleSpecularHDR.dds", m_SpecularIBLSRV, true);
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleBrdf.dds", m_BRDFIBLSRV, false);
	vector<ID3D11ShaderResourceView*> vIBLSRV = { m_EnvIBLSRV.Get(), m_DiffuseIBLSRV.Get(), m_SpecularIBLSRV.Get(), m_BRDFIBLSRV.Get()};
	m_Context->PSSetShaderResources(IBL_TEX_SLOT, IBL_TEX_COUNT, vIBLSRV.data());
	m_Skybox.m_Model = new Model(m_Device, m_Context, { MeshFactory::CreateBox(40) });
	m_Skybox.m_PSO = &SkyboxPSO;

	//ī�޶� ���� ����
	m_Cam.ChangeScreenSize(m_ScreenWidth, m_ScreenHeight);
	m_Cam.GetPTransform()->SetPosition(Vector3(0, 0, -5));

}
void App::UpdateGUI()
{
	//rendering option
	ImGui::Checkbox("MoveMode", &m_MoveMode);
	if (ImGui::TreeNode("IBL"))
	{
		ImGui::SliderFloat("IBL_Strength", &m_GlobalCBufferCPU.iblStrength, 0, 1);
		ImGui::TreePop();
	}
	if (m_SelectedObj != nullptr)
	{
		if (ImGui::TreeNode(m_SelectedObj->m_Name.c_str()))
		{
			Transform* pTF = m_SelectedObj->GetPTransform();
			ImGuiUtil::DrawTransform(m_SelectedObj->GetPTransform());
			ImGuiUtil::DrawMaterial(m_Context, m_SelectedObj->m_PSO->m_MaterialCBufferCPU, m_SelectedObj->m_PSO->m_MaterialCBufferGPU, m_SelectedObj->m_PSO->m_MatKind);
			ImGui::TreePop();
		}
	}

	//ImGui area ignore
	ImVec2 imguiPos = ImGui::GetWindowPos();
	ImVec2 imguiSize = ImGui::GetWindowSize();
	m_ImGuiLTopNS = Vector2(imguiPos.x / m_ScreenWidth, imguiPos.y / m_ScreenHeight);
	m_ImGuiRBotNS = Vector2((imguiPos.x + imguiSize.x) / m_ScreenWidth, (imguiPos.y + imguiSize.y) / m_ScreenHeight);

	//cout << to_string(imguiPos.x) << " " << to_string(imguiPos.y) << " " << to_string(imguiSize.x) << " " << to_string(imguiSize.y) << endl;
	
}
void App::Update(const float deltaTime)
{
	if (isKeyDown(0x46)) //F
	{
		m_MoveMode = !m_MoveMode;
	}
	if (m_MoveMode)
	{
		float camMoveSpeed = 3.0f;
		Transform* pCamTF = m_Cam.GetPTransform();
		Vector3 camPos = pCamTF->GetPosition();
		if (isKey(0x57)) //W
		{
			Vector3 camForward = pCamTF->GetForward();
			pCamTF->SetPosition(camPos + camForward * deltaTime * camMoveSpeed);
		}
		if (isKey(0x41)) //A
		{
			Vector3 camRight = pCamTF->GetRight();
			pCamTF->SetPosition(camPos - camRight * deltaTime * camMoveSpeed);
		}
		if(isKey(0x53)) //S
		{
			Vector3 camForward = pCamTF->GetForward();
			pCamTF->SetPosition(camPos - camForward * deltaTime * camMoveSpeed);
		}
		if (isKey(0x44)) //D
		{
			Vector3 camRight = pCamTF->GetRight();
			pCamTF->SetPosition(camPos + camRight * deltaTime * camMoveSpeed);
		}
		if (isKey(0x20)) //space
		{
			Vector3 camUp = pCamTF->GetUp();
			pCamTF->SetPosition(camPos + camUp * deltaTime * camMoveSpeed);
		}
		if (isKey(0x11)) //leftCtrl
		{
			Vector3 camUp = pCamTF->GetUp();
			pCamTF->SetPosition(camPos - camUp * deltaTime * camMoveSpeed);
		}
		Vector2 mouseMove = GetMouseMove();
		float camRotSpeed = 360.0f;
		Vector3 euler = pCamTF->GetRotation().ToEuler();
		if (mouseMove.y != 0 || mouseMove.x != 0)
		{
			euler.x += mouseMove.y * deltaTime * camRotSpeed;
			euler.y += mouseMove.x * deltaTime * camRotSpeed;
			pCamTF->SetEuler(euler);
		}
	}
	else
	{
		if (m_MouseInGui == false)
		{
			if (isMouseDown(0))
			{
				if (m_MousePickingObjIdx >= 0)
				{
					m_SelectedObj = m_Objs[m_MousePickingObjIdx].get(); //imgui ������ �����ؾ���
					m_MouseisDrag = true;
				}
				else
				{
					m_SelectedObj = nullptr;
				}
			}
			if (m_MouseisDrag && isMouse(0) && isMouseDown(0) == false)
			{
				if (m_SelectedObj != nullptr) 
				{
					Vector2 mouseMove = GetMouseMove();
					if (mouseMove != Vector2::Zero) 
					{
						Transform* pTF = m_SelectedObj->GetPTransform();
						Vector3 worldPos = pTF->GetPosition();
						Vector4 worldV4 = Vector4(worldPos.x, worldPos.y, worldPos.z, 1);

						Matrix viewMat = m_Cam.GetViewMat();
						Matrix projMat = m_Cam.GetProjectionMat();
						Matrix vp = viewMat * projMat;
						Matrix inv_vp = vp.Invert();

						Vector4 clipPos = Vector4::Transform(worldV4, vp);
						clipPos.x = (m_MousePosNS.x * 2) * clipPos.w;
						clipPos.y = 1 - (m_MousePosNS.y * 2) * clipPos.w;
						Vector4 curMouseWS = Vector4::Transform(clipPos, inv_vp);
						clipPos.x = (m_MousePosNS_LastFrame.x * 2) * clipPos.w;
						clipPos.y = 1 - (m_MousePosNS_LastFrame.y * 2) * clipPos.w;
						Vector4 lastMouseWS = Vector4::Transform(clipPos, inv_vp);
						Vector3 mouseMoveWS = Vector3(curMouseWS.x - lastMouseWS.x, curMouseWS.y - lastMouseWS.y, curMouseWS.z - lastMouseWS.z);
						pTF->SetPosition(worldPos + mouseMoveWS);
					}
				}
			}
			if (m_SelectedObj != nullptr)
			{
				
				if (isMouseUp(0))
				{
					m_MouseisDrag = false;
				}
			}
		}
		else
		{
			m_MouseisDrag = false;
		}
	}
	


}

void App::Render()
{
	//Update GlobalCBuffer
	Matrix view = m_Cam.GetViewMat();
	Matrix proj = m_Cam.GetProjectionMat();
	Matrix invProj = proj.Invert();
	Matrix viewProj = view * proj; //row ���
	Matrix invViewProj = viewProj.Invert();
	Vector3 eyeWorld = m_Cam.GetPTransform()->GetPosition();
	m_GlobalCBufferCPU.view = view.Transpose();
	m_GlobalCBufferCPU.proj = proj.Transpose();
	m_GlobalCBufferCPU.invProj = invProj.Transpose();
	m_GlobalCBufferCPU.viewProj = viewProj.Transpose();
	m_GlobalCBufferCPU.invViewProj = invViewProj.Transpose();
	m_GlobalCBufferCPU.eyeWorld = eyeWorld;
	D3DUtil::UpdateCBuffer(m_Context, m_GlobalCBufferCPU, m_GlobalCBufferGPU);

	m_Context->VSSetConstantBuffers(GLOBAL_CBUFFER_SLOT, 1, m_GlobalCBufferGPU.GetAddressOf());
	m_Context->PSSetConstantBuffers(GLOBAL_CBUFFER_SLOT, 1, m_GlobalCBufferGPU.GetAddressOf());


	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_Context->ClearRenderTargetView(m_MainRTV.Get(), clearColor);
	m_Context->ClearDepthStencilView(m_MainDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	m_Context->OMSetRenderTargets(1, m_MainRTV.GetAddressOf(), m_MainDSV.Get());
	
	//IBLTexture

	//��ī�̹ڽ�
	//m_Skybox.Render(m_Context);

	//�⺻ ������Ʈ
	int objCount = m_Objs.size();
	for (int i = 0; i < objCount; i++)
	{
		GameObject* pGO = m_Objs[i].get();
		pGO->Render(m_Context);
	}

	//post effect

	//post processing
	//Mouse Picking
	if (EditMode)
	{
		//Mousepicking draw
		m_Context->ClearRenderTargetView(m_MousePickingRTV.Get(), clearColor);
		m_Context->ClearDepthStencilView(m_MainDSV.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
		m_Context->OMSetRenderTargets(1, m_MousePickingRTV.GetAddressOf(), m_MainDSV.Get());
		MousePickingPSO.RenderSetting(m_Context);
		MousePickingCBuffer* cbufferCPU = static_cast<MousePickingCBuffer*>(MousePickingPSO.m_MaterialCBufferCPU);
		UINT r = 0, g = 0, b = 0;
		for (int i = 0; i < objCount; i++)
		{
			Util::IdxToUINT3Color(i + 1, r, g, b);
			cbufferCPU->idx = i;
			cbufferCPU->color[0] = r;
			cbufferCPU->color[1] = g;
			cbufferCPU->color[2] = b;
			MousePickingPSO.UpdateMatCBuffer(m_Context);
			GameObject* pGO = m_Objs[i].get();
			pGO->RenderUseCustomPSO(m_Context);
		}
		m_Context->OMSetRenderTargets(1, m_MainRTV.GetAddressOf(), m_MainDSV.Get());
		m_Context->CopyResource(m_MousePickingStagingTex.Get(), m_MousePickingTex.Get());

		//Ignore ImGui area
		if (m_MousePosNS.x >= m_ImGuiLTopNS.x && m_MousePosNS.x <= m_ImGuiRBotNS.x && m_MousePosNS.y >= m_ImGuiLTopNS.y && m_MousePosNS.y <= m_ImGuiRBotNS.y)
		{
			m_MouseInGui = true;
		}	
		else
		{
			m_MouseInGui = false;

			//Copy to stagingTex
			D3D11_MAPPED_SUBRESOURCE ms;
			HRESULT hr = m_Context->Map(m_MousePickingStagingTex.Get(), 0, D3D11_MAP_READ, 0, &ms);
			if (SUCCEEDED(hr))
			{
				//Get texel
				int screen_x = 0, screen_y = 0;
				Util::NDCToScreen(m_MousePosNS, m_ScreenWidth, m_ScreenHeight, screen_x, screen_y);
				uint8_t* pixel = static_cast<uint8_t*>(ms.pData) + screen_x * 4 + screen_y * ms.RowPitch;
				uint8_t r = pixel[0];
				uint8_t g = pixel[1];
				uint8_t b = pixel[2];
				m_MousePickingObjIdx = Util::UINT3ColorToIdx(r, g, b) - 1;
			}
			else
			{
				m_MousePickingObjIdx = -1;
			}
			m_Context->Unmap(m_MousePickingStagingTex.Get(), 0);
		}
	}

	
}


void App::LoadTexture(const string& filename, const bool useSRGB, ComPtr<ID3D11Texture2D>& tex, ComPtr<ID3D11ShaderResourceView>& srv, const UINT mipLevel)
{
	D3DUtil::CreateTexture(m_Device, m_Context, filename, useSRGB, tex, srv, mipLevel);

	size_t nameKey = std::hash<string>()(filename);
	m_TexViews[nameKey] = srv;
}
void App::LoadModel(const string& modelName, MeshData meshData)
{
	size_t nameKey = std::hash<string>()(modelName);
	m_Models[nameKey] = make_unique<Model>(Model(m_Device, m_Context, { meshData }));
}
GameObject* App::CreateObj(const string& name, const string& modelName, GraphicsPSO* pPSO)
{
	m_Objs.push_back(make_shared<GameObject>(GameObject(m_Device, name, GetModel(modelName), pPSO)));
	return m_Objs.back().get();
}

GameObject* App::GetObj(const string& name)
{
	UINT size = m_Objs.size();
	size_t nameKey = std::hash<string>()(name);
	for (int i = 0; i < size; i++)
	{
		GameObject* pGO = m_Objs[i].get();
		if (pGO->m_NameKey == nameKey)
		{
			return pGO;
		}
	}
	return nullptr;
}

Model* App::GetModel(const string& name)
{
	UINT size = m_Objs.size();
	size_t nameKey = std::hash<string>()(name);
	if (m_Models.find(nameKey) != m_Models.end())
	{
		return m_Models[nameKey].get();
	}
	return nullptr;
}

ID3D11ShaderResourceView* App::GetTexView(const string& name)
{
	UINT size = m_Objs.size();
	size_t nameKey = std::hash<string>()(name);
	if (m_TexViews.find(nameKey) != m_TexViews.end())
	{
		return m_TexViews[nameKey].Get();
	}
	return nullptr;
}

const bool App::isKey(UINT key) const
{
	if (key < 256)
	{
		return m_KeyDown[key];
	}
	return false;
}

const bool App::isKeyDown(UINT key) const
{
	if (key < 256)
	{
		return m_KeyDown[key] && m_KeyDown_LastFrame[key] == false;
	}
	return false;
}

const bool App::isKeyUp(UINT key) const
{
	if (key < 256)
	{
		return m_KeyDown[key] == false && m_KeyDown_LastFrame[key];
	}
	return false;
}
const bool App::isMouseDown(UINT button)const
{
	if (button < 2) //0 = left, 1 = right
	{
		return m_MouseDown[button] && m_MouseDown_LastFrame[button] == false;
	}
}
const bool App::isMouseUp(UINT button)const
{
	if (button < 2) //0 = left, 1 = right
	{
		return m_MouseDown[button] == false && m_MouseDown_LastFrame[button];
	}
}
const bool App::isMouse(UINT button)const
{
	if (button < 2) //0 = left, 1 = right
	{
		return m_MouseDown[button];
	}
}
const Vector2 App::GetMouseMove()const
{
	return m_MousePosNS - m_MousePosNS_LastFrame;
}

bool App::Init()
{
	//winapi init
	if (InitWindow() == false)
	{
		return false;
	}

	//directx init
	if (InitDirect3D() == false)
	{
		return false;
	}

	//imgui init
	if (InitGUI() == false)
	{
		return false;
	}

	InitGraphics(m_Device, m_Context);

	return true;
}
int App::Run()
{
	Start();

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		ImGui::Begin("Scene Control");

		ImGui::Text("Average %.3f ms/ framge (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		UpdateGUI();

		ImGui::End();
		ImGui::Render();

		Update(ImGui::GetIO().DeltaTime);
		Render();

		for (int i = 0; i < 256; i++)
		{
			m_KeyDown_LastFrame[i] = m_KeyDown[i];
		}
		m_MousePosNS_LastFrame = m_MousePosNS;
		m_MouseDown_LastFrame[0] = m_MouseDown[0];
		m_MouseDown_LastFrame[1] = m_MouseDown[1];

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		m_SwapChain->Present(1, 0);
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return App::pIns->MsgProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK App::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam))
	{
		return true;
	}

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_DISPLAYCHANGE:
		break;
	case WM_SIZE:
		if (m_SwapChain)
		{
			//���⼱ �������� ũ�Ⱑ ���� �״�� �����
			int width = lParam & 0x0000FFFF;
			int height = (lParam >> 16) & 0x0000FFFF;
			m_ScreenWidth = width;
			m_ScreenHeight = height;
			m_Cam.ChangeScreenSize(m_ScreenWidth, m_ScreenHeight);

			m_MainRTV.Reset();
			m_MainSRV.Reset();
			m_MainDSV.Reset();

			if (EditMode) 
			{
				m_MousePickingRTV.Reset();
				m_MousePickingStagingTex.Reset();
				m_MousePickingTex.Reset();
			}

			m_SwapChain->ResizeBuffers(0, //���� ���� ����
				m_ScreenWidth, m_ScreenHeight, DXGI_FORMAT_UNKNOWN, //���� ���� ����
				0);

			D3DUtil::SetViewport(m_Context, m_ScreenViewport, m_ScreenWidth, m_ScreenHeight);
			D3DUtil::CreateRenderTargetShaderResourceView(m_Device, m_SwapChain, m_MainRTV, m_MainSRV);
			D3DUtil::CreateDepthBuffer(m_Device, m_MainDSV, m_ScreenWidth, m_ScreenHeight, numQualityLevels);
			if (EditMode)
			{
				D3DUtil::CreateMousePickingResources(m_Device, m_MousePickingTex, m_MousePickingRTV, m_MousePickingStagingTex, m_ScreenWidth, m_ScreenHeight);
			}

			//ī�޶� ���� ����
		}
		break;
	case WM_MOUSEMOVE:
		//�»�� (0,0), ���ϴ� = screen size  => �»�� (0,0) ���ϴ� (1,1) NormalizedScreen
		m_MousePosNS.x = GET_X_LPARAM(lParam) / (float)m_ScreenWidth;
		m_MousePosNS.y = GET_Y_LPARAM(lParam) / (float)m_ScreenHeight;
		//cout << to_string(m_MousePosNS.x) <<" " << to_string(m_MousePosNS.y) << endl;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		m_MouseDown[0] = true;
		break;
	case WM_LBUTTONUP:
		m_MouseDown[0] = false;
		break;
	case WM_RBUTTONDOWN:
		m_MouseDown[1] = true;
		break;
	case WM_RBUTTONUP:
		m_MouseDown[1] = false;
		break;
	case WM_KEYDOWN:
		m_KeyDown[wParam] = true;
		break;
	case WM_KEYUP:
		m_KeyDown[wParam] = false;
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
bool App::InitWindow()
{
	const LPCWSTR lpClassName = TEXT("class");
	const LPCWSTR lpTitleName = TEXT("Title");

	WNDCLASSEX wndc;
	wndc.cbSize = sizeof(wndc);
	wndc.style = CS_HREDRAW | CS_VREDRAW;
	wndc.lpfnWndProc = WndProc;
	wndc.cbClsExtra = NULL;
	wndc.cbWndExtra = NULL;
	wndc.hInstance = GetModuleHandle(NULL);
	wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndc.lpszMenuName = NULL;
	wndc.lpszClassName = lpClassName;
	wndc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndc))
	{
		cout << "CreateScreen in RegisterClassEx failed" << endl;
		return false;
	}

	//�������� �ػ󵵸� width �� height �� ���߱� ���� ���
	RECT rect = { 0,0,m_ScreenWidth, m_ScreenHeight };

	//�������� width �� height �� ����� ���� ���� ��� �� rect�� width �� height �� ����
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	m_HWND = CreateWindow(
		lpClassName,
		lpTitleName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, //������ ���� ��� x ��ǥ
		CW_USEDEFAULT, //������ ���� ��� y ��ǥ
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		wndc.hInstance,
		NULL
	);

	if (m_HWND == 0)
	{
		cout << "CreateWindow failed" << endl;
		return false;
	}

	ShowWindow(m_HWND, SW_SHOWDEFAULT);
	UpdateWindow(m_HWND);

	return true;
}
bool App::InitDirect3D()
{
	//GPU, CPU �� �� ������ ���°� HARDWARE �� ���� ������. �ϵ���� ���� ���ο� ���� �ٸ��� �����ؾ��ϱ⵵ ��
	const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

	//����� ��� ��� �� ����׸� �����ϴ� ����̽��� �����.
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG) || defined(asdf)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	//������� �ϵ��� ���������� �������� ���� ��� �ܰ踦 ���簡�� �����Ǵ°� ã�°��� ����
	//����� 11�� ������ �����Ѵٴ� �������� ����

	const D3D_FEATURE_LEVEL featureLevels[3] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D_FEATURE_LEVEL featureLevel;

	if (FAILED(D3D11CreateDevice(
		nullptr,    // Specify nullptr to use the default adapter.
		driverType, // Create a device using the hardware graphics driver.
		0, // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		createDeviceFlags, // Set debug and Direct2D compatibility flags.
		featureLevels,     // List of feature levels this app can support.
		ARRAYSIZE(featureLevels), // Size of the list above.
		D3D11_SDK_VERSION,     // Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
		m_Device.GetAddressOf(), // Returns the Direct3D device created.
		&featureLevel,         // Returns feature level of device created.
		m_Context.GetAddressOf() // Returns the device immediate context.
	))) 
	{
		cout << "D3D11CreateDevice failed." << endl;
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		cout << "D3D Feature Level 11 unsupported" << endl;
		return false;
	}

	//4X MSAA �����ϴ��� Ȯ��
	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
	if (numQualityLevels <= 0)
	{
		cout << "MSAA not supported numQualityLevel : " << numQualityLevels << endl;
		return false;
	}
	numQualityLevels = 0;
	//����ü��
	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
	swapchainDesc.BufferDesc.Width = m_ScreenWidth;
	swapchainDesc.BufferDesc.Height = m_ScreenHeight;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //UNORM = 8��Ʈ�� ���� �� ����� �� 0~1 ������ float �� ��ȯ �� ���
	swapchainDesc.BufferCount = 2; //2�߹���

	swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;	//60fps
	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapchainDesc.BufferUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = m_HWND;
	swapchainDesc.Windowed = true; //â��� = true, ��üȭ�� = false
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //��üȭ�� ��ȯ�� �ػ� ���� ���� ���ߵ� �ȴٴµ� �𸣰���
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //���� �׸��� �� �� ���۳��� ���
	if (numQualityLevels > 0)
	{
		swapchainDesc.SampleDesc.Count = 4; //����2�� ����2�� �÷��� ��� �� �����ؼ� ���
		swapchainDesc.SampleDesc.Quality = numQualityLevels - 1;
	}
	else
	{
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;
	}

	if (FAILED(D3D11CreateDeviceAndSwapChain
	(0, driverType, 0, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapchainDesc,
		m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(), &featureLevel, m_Context.GetAddressOf())))
	{
		cout << "D3D11CreateDeviceAndSwapCahin failed" << endl;
		return false;
	}

	D3DUtil::SetViewport(m_Context, m_ScreenViewport, m_ScreenWidth, m_ScreenHeight);
	if (!D3DUtil::CreateRenderTargetShaderResourceView(m_Device, m_SwapChain, m_MainRTV, m_MainSRV)) { return false; }
	if (!D3DUtil::CreateDepthBuffer(m_Device, m_MainDSV, m_ScreenWidth, m_ScreenHeight, numQualityLevels)) { return false; }

	if (EditMode)
	{
		D3DUtil::CreateMousePickingResources(m_Device, m_MousePickingTex, m_MousePickingRTV, m_MousePickingStagingTex, m_ScreenWidth, m_ScreenHeight);
	}

	D3DUtil::CreateCBuffer(m_Device, m_GlobalCBufferCPU, m_GlobalCBufferGPU);


	return true;
}
bool App::InitGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui::GetIO().DisplaySize = ImVec2(m_ScreenWidth, m_ScreenHeight);

	if (!ImGui_ImplDX11_Init(m_Device.Get(), m_Context.Get()))
	{
		return false;
	}

	if (!ImGui_ImplWin32_Init(m_HWND))
	{
		return false;
	}

	return true;
}
