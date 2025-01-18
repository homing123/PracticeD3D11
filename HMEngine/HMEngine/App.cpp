
#include "App.h"
const float PI = 3.14159265f;
const float ToDegree = 180 / PI;
const float ToRadian = PI / 180;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


App* App::pIns = nullptr;

void App::Start()
{

	//texture
	ComPtr<ID3D11Texture2D> tex;
	ComPtr<ID3D11ShaderResourceView> srv;
	LoadTexture("../Resource/Tex/UVTex.png", false, tex, srv, 1);
	LoadModel("Sphere", MeshFactory::CreateSphere(1, 10, 10));
	CreateObj("Sphere", "Sphere", &BlinnPhongPSO_0);

	//skybox
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleEnvHDR.dds", m_EnvIBLSRV, true);
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleDiffuseHDR.dds", m_DiffuseIBLSRV, true);
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleSpecularHDR.dds", m_SpecularIBLSRV, true);
	D3DUtil::CreateDDSTexture(m_Device, L"../Resource/CubeMap/SampleBrdf.dds", m_BRDFIBLSRV, false);
	vector<ID3D11ShaderResourceView*> vIBLSRV = { m_EnvIBLSRV.Get(), m_DiffuseIBLSRV.Get(), m_SpecularIBLSRV.Get(), m_BRDFIBLSRV.Get()};
	m_Context->PSSetShaderResources(IBL_TEX_SLOT, IBL_TEX_COUNT, vIBLSRV.data());
	m_Skybox.m_Model = new Model(m_Device, m_Context, { MeshFactory::CreateBox(40) });
	m_Skybox.m_PSO = &SkyboxPSO;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ComPtr<ID3D11Texture2D> texture;
	m_EnvIBLSRV.Get()->GetResource((ID3D11Resource**)texture.GetAddressOf());
	D3D11_TEXTURE2D_DESC desc2d;
	texture.Get()->GetDesc(&desc2d);
	UINT width = desc2d.Width;

	
	//ī�޶� ���� ����
	m_Cam.ChangeScreenSize(m_ScreenWidth, m_ScreenHeight);
	m_Cam.GetPTransform()->SetPosition(Vector3(0, 0, -5));



	//m_CubeMap.Init(m_Device.Get());

	////postprocessing 

	//m_Filters.clear();
	//auto copyFilter = make_shared<ImageFilter>();
	//copyFilter->Init(m_Device.Get(), m_Context.Get(), GetMesh(ImageFilter::MeshName), L"PSSampling.hlsl", m_ScreenWidth, m_ScreenHeight);
	//copyFilter->SetShaderResources({ m_ShaderResourceView });
	//m_Filters.push_back(copyFilter);

	//auto bloomSampleFilter = make_shared<ImageFilter>();
	//bloomSampleFilter->Init(m_Device.Get(), m_Context.Get(), GetMesh(ImageFilter::MeshName), L"PSSampling.hlsl", m_ScreenWidth, m_ScreenHeight);
	//bloomSampleFilter->SetShaderResources({ m_ShaderResourceView });
	//m_Filters.push_back(bloomSampleFilter);

	//int width = m_ScreenWidth;
	//int height = m_ScreenHeight;

	////�ٿ���ø�
	//for (int i = 0; i < 4; i++)
	//{
	//	width /= 2;
	//	height /= 2;
	//	auto filter = make_shared<ImageFilter>();
	//	filter->Init(m_Device.Get(), m_Context.Get(), GetMesh(ImageFilter::MeshName), L"PSSampling.hlsl", width, height);
	//	filter->SetShaderResources({ m_Filters.back().get()->m_ShaderResourceView });
	//	m_Filters.push_back(filter);
	//}

	////���� �� �����ø�
	//for (int i = 0; i < 4; i++)
	//{
	//	auto filter = make_shared<ImageFilter>();
	//	filter.get()->Init(m_Device.Get(), m_Context.Get(), GetMesh(ImageFilter::MeshName), L"PSBlurX.hlsl", width, height);
	//	filter.get()->SetShaderResources({ m_Filters.back().get()->m_ShaderResourceView });
	//	m_Filters.push_back(filter);

	//	filter = make_shared<ImageFilter>();
	//	filter.get()->Init(m_Device.Get(), m_Context.Get(), GetMesh(ImageFilter::MeshName), L"PSBlurY.hlsl", width, height);
	//	filter.get()->SetShaderResources({ m_Filters.back().get()->m_ShaderResourceView });
	//	m_Filters.push_back(filter);
	//	width *= 2;
	//	height *= 2;

	//	filter = make_shared<ImageFilter>();
	//	filter->Init(m_Device.Get(), m_Context.Get(), GetMesh(ImageFilter::MeshName), L"PSSampling.hlsl", width, height);
	//	filter->SetShaderResources({ m_Filters.back().get()->m_ShaderResourceView });
	//	m_Filters.push_back(filter);
	//}

	//auto finalFilter = make_shared<ImageFilter>();
	//finalFilter.get()->Init(m_Device.Get(), m_Context.Get(), GetMesh(ImageFilter::MeshName), L"PSCombine.hlsl", m_ScreenWidth, m_ScreenHeight);
	//finalFilter.get()->SetShaderResources({ copyFilter->m_ShaderResourceView, m_Filters.back().get()->m_ShaderResourceView });
	//finalFilter.get()->SetRenderTargets({ m_RenderTargetView });
	//m_Filters.push_back(finalFilter);

}
void App::UpdateGUI()
{
	//rendering option
	ImGui::Checkbox("MoveMode", &m_MoveMode);
	
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
		cout << mouseMove.x << mouseMove.y << endl;
		Vector3 euler = pCamTF->GetRotation().ToEuler();
		if (mouseMove.y != 0 || mouseMove.x != 0)
		{
			euler.x -= mouseMove.y * deltaTime * camRotSpeed;
			euler.y += mouseMove.x * deltaTime * camRotSpeed;
			pCamTF->SetEuler(euler);
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


	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_Context->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
	m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	m_Context->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	
	//IBLTexture

	//��ī�̹ڽ�
	m_Skybox.Render(m_Context);

	//�⺻ ������Ʈ
	int objCount = m_Objs.size();
	for (int i = 0; i < objCount; i++)
	{
		GameObject* pGO = m_Objs[i].get();
		pGO->Render(m_Context);
	}

	//post effect

	//post processing


	//cubeMap

	//ID3D11ShaderResourceView* arr_pView[1] = { m_CubeMap.m_SpecularResourceView.Get()};
	//m_Context->IASetInputLayout(m_CubeMap.m_InputLayout.Get());
	//m_Context->IASetVertexBuffers(0, 1, pCubeMapMesh->m_VertexBuffer.GetAddressOf(), &stride, &offset);
	//m_Context->IASetIndexBuffer(pCubeMapMesh->m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//m_Context->VSSetShader(m_CubeMap.m_VS.Get(), 0, 0);
	//m_Context->PSSetShader(m_CubeMap.m_PS.Get(), 0, 0);
	//m_CubeMap.UpdateVSCBuffer(m_Context.Get(), MatView, MatProj);
	//m_Context->PSSetShaderResources(0, 1, arr_pView);
	//m_Context->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
	//m_Context->VSSetConstantBuffers(0, 1, m_CubeMap.m_VSCBuffer.GetAddressOf());
	//m_Context->PSSetConstantBuffers(0, 1, m_CubeMap.m_PSCBuffer.GetAddressOf());
	//m_Context->DrawIndexed(pCubeMapMesh->GetIndexCount(), 0, 0);

	//PSCBuffer psCBuffer;
	//psCBuffer.eyeWorld = m_Cam.GetPTransform()->GetPosition();
	//// ���� �� ���� ��� ����
	//for (int i = 0; i < MAX_LIGHTS; i++) {
	//	// �ٸ� ���� ����
	//	if (i != m_LightType) {
	//		psCBuffer.lights[i].strength *= 0.0f;
	//	}
	//	else {
	//		psCBuffer.lights[i] = m_Light;
	//	}
	//}
	//psCBuffer.useTexture = m_UseTexture;
	//psCBuffer.material.ambient = Vector3(1, 1, 1) * m_MatAmbient;
	//psCBuffer.material.diffuse = Vector3(1, 1, 1) * m_MatDiffuse;
	//psCBuffer.material.specular = Vector3(1, 1, 1) * m_MatSpecular;
	//psCBuffer.material.shininess = m_MatShininess;
	//psCBuffer.rimColor = m_RimColor;
	//psCBuffer.rimPower = m_RimPower;
	//psCBuffer.rimStrength = m_RimStrength;

	//size_t texKey = hash<string>()("wall");
	//ID3D11ShaderResourceView* texViews[3] = { m_TexViews[texKey].Get(), m_CubeMap.m_DiffuseResourceView.Get(), m_CubeMap.m_SpecularResourceView.Get()};
	//ID3D11SamplerState* samplers[1] = { m_SamplerState.Get() };
	//for (int i = 0; i < objCount; i++)
	//{
	//	
	//	GameObject* pGO = m_Objs[i].get();
	//	pGO->Render(m_Context.Get(), MatView, MatProj, psCBuffer, texViews, samplers);
	//	
	//}

	
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

const Vector2 App::GetMouseMove()const
{
	return m_MousePos - m_MousePos_LastFrame;
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
		m_MousePos_LastFrame = m_MousePos;

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
			m_RenderTargetView.Reset();
			m_SwapChain->ResizeBuffers(0, //���� ���� ����
				m_ScreenWidth, m_ScreenHeight, DXGI_FORMAT_UNKNOWN, //���� ���� ����
				0);

			/*CreateRenderTargetView();
			CreateDepthBuffer();
			SetViewport();*/

			D3DUtil::SetViewport(m_Context.Get(), m_ScreenViewport, m_ScreenWidth, m_ScreenHeight);
			D3DUtil::CreateRenderTargetShaderResourceView(m_Device, m_SwapChain, m_RenderTargetView, m_ShaderResourceView);
			D3DUtil::CreateDepthBuffer(m_Device, m_DepthStencilView, m_ScreenWidth, m_ScreenHeight, numQualityLevels);

			//ī�޶� ���� ����
		}
		break;
	case WM_MOUSEMOVE:
		//�»�� (0,0), ���ϴ� = screen size  => ���ϴ� (0,0) ���� (1,1)
		m_MousePos.x = GET_X_LPARAM(lParam) / (float)m_ScreenWidth;
		m_MousePos.y = 1 - GET_Y_LPARAM(lParam) / (float)m_ScreenHeight;
		//cout << to_string(m_MousePos.x) <<" " << to_string(m_MousePos.y) << endl;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
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
	//GPU, CPU �� �� ������ �����°� HARDWARE �� ���� ������. �ϵ���� ���� ���ο� ���� �ٸ��� �����ؾ��ϱ⵵ ��
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
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //���� �׸��� �� �� ���۳��� ���
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

	D3DUtil::SetViewport(m_Context.Get(), m_ScreenViewport, m_ScreenWidth, m_ScreenHeight);
	if (!D3DUtil::CreateRenderTargetShaderResourceView(m_Device, m_SwapChain, m_RenderTargetView, m_ShaderResourceView)) { return false; }
	if (!D3DUtil::CreateDepthBuffer(m_Device, m_DepthStencilView, m_ScreenWidth, m_ScreenHeight, numQualityLevels)) { return false; }

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