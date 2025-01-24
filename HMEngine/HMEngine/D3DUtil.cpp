#define STB_IMAGE_IMPLEMENTATION
#include "D3DUtil.h"


void D3DUtil::CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<Vertex>& hBuffer, ComPtr<ID3D11Buffer>& dBuffer)
{
	//버텍스 버퍼
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; //초기화 후 변경 X
	vertexBufferDesc.ByteWidth = UINT(sizeof(Vertex) * hBuffer.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vBufferData = { 0 };
	vBufferData.pSysMem = hBuffer.data();
	vBufferData.SysMemPitch = 0; //텍스쳐 줄 시작 에서 다음 줄 까지의 거리	2D, 3D 텍스쳐만 사용
	vBufferData.SysMemSlicePitch = 0; //텍스쳐 깊이 시작에서 다음 깊이 까지의 거리 3D 텍스쳐만 사용

	HRESULT hResult = device->CreateBuffer(&vertexBufferDesc, &vBufferData, dBuffer.GetAddressOf());
	if (FAILED(hResult))
	{
		cout << " Create VertexBuffer Failed" << endl;
		return;
	}
}
void D3DUtil::CreateBillboardVertexIndexBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Buffer>& vertexBuffer,ComPtr<ID3D11Buffer>& indexBuffer)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; //초기화 후 변경 X
	vertexBufferDesc.ByteWidth = UINT(sizeof(Vector3));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.StructureByteStride = sizeof(Vector3);

	D3D11_SUBRESOURCE_DATA vBufferData = { 0 };
	vector<Vector3> vertexData = { Vector3::Zero };
	vBufferData.pSysMem = vertexData.data();
	vBufferData.SysMemPitch = 0;
	vBufferData.SysMemSlicePitch = 0;

	HRESULT hResult = device->CreateBuffer(&vertexBufferDesc, &vBufferData, vertexBuffer.GetAddressOf());
	if (FAILED(hResult))
	{
		cout << " Create Billboard VertexBuffer Failed" << endl;
		return;
	}

	//인덱스 버퍼
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = UINT(sizeof(UINT32));
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.StructureByteStride = sizeof(UINT32);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	vector<UINT32> indexData = { 0 };
	indexBufferData.pSysMem = indexData.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	hResult = device->CreateBuffer(&indexBufferDesc, &indexBufferData, indexBuffer.GetAddressOf());
	if (FAILED(hResult))
	{
		cout << "Create Billboard IndexBuffer Failed" << endl;
		return;
	}
}

void D3DUtil::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& hBuffer, ComPtr<ID3D11Buffer>& dBuffer)
{

	//인덱스 버퍼
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = UINT(sizeof(UINT32) * hBuffer.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.StructureByteStride = sizeof(UINT32);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = hBuffer.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	HRESULT hResult = device->CreateBuffer(&indexBufferDesc, &indexBufferData, dBuffer.GetAddressOf());
	if (FAILED(hResult))
	{
		cout << "Create IndexBuffer Failed" << endl;
		return;
	}
}

bool D3DUtil::CreateRenderTargetShaderResourceView(ComPtr<ID3D11Device>& device, ComPtr<IDXGISwapChain>& swapChain, ComPtr<ID3D11RenderTargetView>& renderTargetView, ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
	ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	if (backBuffer)
	{
		device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
		device->CreateShaderResourceView(backBuffer.Get(), nullptr, shaderResourceView.GetAddressOf());
	}
	else
	{
		cout << "CreateRenderTargetShaderResourceView failed" << endl;
		return false;
	}
	return true;
}

void D3DUtil::CreateMousePickingResources(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Texture2D>& rtvTex, ComPtr<ID3D11RenderTargetView>& rtv, ComPtr<ID3D11Texture2D>& stagingTex, const UINT width, const UINT height)
{
	//RTV
	D3D11_TEXTURE2D_DESC rtvTexDesc;
	ZeroMemory(&rtvTexDesc, sizeof(rtvTexDesc));
	rtvTexDesc.Width = width;
	rtvTexDesc.Height = height;
	rtvTexDesc.ArraySize = 1;
	rtvTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
	rtvTexDesc.CPUAccessFlags = 0;
	rtvTexDesc.MipLevels = 1;
	rtvTexDesc.Usage = D3D11_USAGE_DEFAULT;
	rtvTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	rtvTexDesc.MiscFlags = 0;
	rtvTexDesc.SampleDesc.Count = 1;
	rtvTexDesc.SampleDesc.Quality = 0;
	device->CreateTexture2D(&rtvTexDesc, nullptr, rtvTex.GetAddressOf());

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//rtvDesc.Buffer = D3D11_BUFFER_RTV();  //ViewDimension 이 	D3D11_RTV_DIMENSION_BUFFER 인 경우의 정의
	device->CreateRenderTargetView(rtvTex.Get(), &rtvDesc, rtv.GetAddressOf());


	//staging Texture
	rtvTexDesc.Usage = D3D11_USAGE_STAGING;
	rtvTexDesc.BindFlags = 0; //staging tex
	rtvTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	device->CreateTexture2D(&rtvTexDesc, nullptr, stagingTex.GetAddressOf());
}

bool D3DUtil::CreateDepthBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DepthStencilView>& depthStencilView, const UINT width, const UINT height, const int qualitylevels)
{
	//깊이 스텐실 뷰 생성
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (qualitylevels > 0)
	{
		depthStencilBufferDesc.SampleDesc.Count = 4;
		depthStencilBufferDesc.SampleDesc.Quality = qualitylevels - 1;
	}
	else
	{
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;

	if (FAILED(device->CreateTexture2D(&depthStencilBufferDesc, 0, depthStencilBuffer.GetAddressOf())))
	{
		cout << "CreateTexture2D DepthStencilBuffer failed" << endl;
		return false;
	}

	if (FAILED(device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf())))
	{
		cout << "CreateDepthStencilView failed" << endl;
		return false;
	}
	return true;
}
void D3DUtil::SetViewport(ComPtr<ID3D11DeviceContext>& context, D3D11_VIEWPORT& screenViewport, const UINT width, const UINT height)
{
	ZeroMemory(&screenViewport, sizeof(screenViewport));
	screenViewport.Width = width;
	screenViewport.Height = height;
	screenViewport.TopLeftX = 0;
	screenViewport.TopLeftY = 0;
	screenViewport.MinDepth = 0.f;
	screenViewport.MaxDepth = 1.f;
	context->RSSetViewports(1, &screenViewport);
}
void D3DUtil::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& fileName, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vs, ComPtr<ID3D11InputLayout>& inputLayout)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
		{
			wcout << fileName.c_str() << " File not Found" << endl;
			return;
		}

		if (errorBlob)
		{
			wcout << fileName.c_str() << " Compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
			return;
		}

		wcout << fileName.c_str() << " Unknown error in VSBasic shader compile" << endl;
		return;
	}
	
	hr = device->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << fileName.c_str() << " CreateInputLayout Failed" << endl;
	}
	hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, vs.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << fileName.c_str() << " CreateVertexShader Failed" << endl;
	}

}

void D3DUtil::CreateHullShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11HullShader>& hs)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "hs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
		{
			wcout << fileName.c_str() << "File not Found" << endl;
			return;
		}

		if (errorBlob)
		{
			wcout << fileName.c_str() << "Compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
			return;
		}

		wcout << fileName.c_str() << " Unknown error in VSBasic shader compile" << endl;
		return;
	}

	device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, hs.GetAddressOf());
}
void D3DUtil::CreateDomainShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11DomainShader>& ds)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ds_5_0", compileFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
		{
			wcout << fileName.c_str() << "File not Found" << endl;
			return;
		}

		if (errorBlob)
		{
			wcout << fileName.c_str() << "Compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
			return;
		}

		wcout << fileName.c_str() << " Unknown error in VSBasic shader compile" << endl;
		return;
	}

	device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, ds.GetAddressOf());
}

void D3DUtil::CreateGeometryShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11GeometryShader>& gs)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
		{
			wcout << fileName.c_str() << "File not Found" << endl;
			return;
		}

		if (errorBlob)
		{
			wcout << fileName.c_str() << "Compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
			return;
		}

		wcout << fileName.c_str() << " Unknown error in VSBasic shader compile" << endl;
		return;
	}

	device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, gs.GetAddressOf());
}

void D3DUtil::CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11PixelShader>& ps)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
		{
			wcout << fileName.c_str() << "File not Found" << endl;
			return;
		}

		if (errorBlob)
		{
			wcout << fileName.c_str() << "Compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
			return;
		}

		wcout << fileName.c_str() << " Unknown error in VSBasic shader compile" << endl;
		return;
	}

	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, ps.GetAddressOf());
}


void D3DUtil::CreateDDSTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& srv, bool isCubemap) 
{
	ComPtr<ID3D11Texture2D> texture;
	UINT miscflag = isCubemap ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
	ThrowFail(CreateDDSTextureFromFileEx(device.Get(), filename, 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE, 0, miscflag, DDS_LOADER_DEFAULT, (ID3D11Resource**)texture.GetAddressOf(), srv.GetAddressOf()));
	wcout << "CreateDDSTexture Success : " << filename << endl;
}

void D3DUtil::CreateTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const string& filename, const bool useSRGB,
	ComPtr<ID3D11Texture2D>& tex, ComPtr<ID3D11ShaderResourceView>& srv, const UINT mipLevel)
{
	int width, height;

	//포멧 exr 일때는 다르게 하던데 필요할때 추가하자

	vector<uint8_t> texBuffer;
	ReadTexture(filename, texBuffer, width, height);

	//SRGB 란 감마보정과 관련
	//인간의 눈은 어두운곳을 더 민감하게 반응한다 즉 선형적인 색깔변화를 선형적으로 받아들이지 못한다.
	//즉 SRGB처리된건 감마보정이 처리된 값이 들어가있는거다.
	DXGI_FORMAT format = useSRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM; 

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	if (mipLevel == 1)
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.MipLevels = 1;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = texBuffer.data();
		initData.SysMemPitch = desc.Width * sizeof(uint8_t) * 4;
		initData.SysMemSlicePitch = 0;

		ThrowFail(device->CreateTexture2D(&desc, &initData, tex.GetAddressOf()));
		ThrowFail(device->CreateShaderResourceView(tex.Get(), nullptr, srv.GetAddressOf()));
	}
	else
	{

	}
	cout << "Create Texture Success" << endl;
}

void D3DUtil::ReadTexture(const string& filename, vector<uint8_t>& out_texBuffer, int& out_width, int& out_height)
{
	int width, height, channels;

	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0); //unsigned char = 8비트
	cout << "TexLoad file : " << filename << " w, h, channel : " << width << ", " << height << ", " << channels << endl;
	if (width * height == 0)
	{
		cout << "TexLoadFail buffer error" << channels << endl;
		return;
	}
	out_width = width;
	out_height = height;
	out_texBuffer.resize(out_width * out_height * 4); //4채널 크기로 통일해서 사용

	switch (channels)
	{
	case 1: //4채널 모두 같은 값으로 채움
		for (UINT i = 0; i < width * height; i++)
		{
			out_texBuffer[i * 4 + 0] = img[i];
			out_texBuffer[i * 4 + 1] = img[i];
			out_texBuffer[i * 4 + 2] = img[i];
			out_texBuffer[i * 4 + 3] = img[i];
		}
		break;
	case 2: //빈채널 모두 1로 채움
		for (UINT i = 0; i < width * height; i++)
		{
			out_texBuffer[i * 4 + 0] = img[i * channels + 0];
			out_texBuffer[i * 4 + 1] = img[i * channels + 1];
			out_texBuffer[i * 4 + 2] = 255;
			out_texBuffer[i * 4 + 3] = 255;
		}
		break;
	case 3: //빈채널 모두 1로 채움
		for (UINT i = 0; i < width * height; i++)
		{
			out_texBuffer[i * 4 + 0] = img[i * channels + 0];
			out_texBuffer[i * 4 + 1] = img[i * channels + 1];
			out_texBuffer[i * 4 + 2] = img[i * channels + 2];
			out_texBuffer[i * 4 + 3] = 255;
		}
		break;
	case 4:
		for (UINT i = 0; i < width * height; i++)
		{
			out_texBuffer[i * 4 + 0] = img[i * channels + 0];
			out_texBuffer[i * 4 + 1] = img[i * channels + 1];
			out_texBuffer[i * 4 + 2] = img[i * channels + 2];
			out_texBuffer[i * 4 + 3] = img[i * channels + 3];
		}
		break;
	default:
		cout << "TexLoadFail channel error" << channels << endl;
		return;
	}

	delete[] img;
}