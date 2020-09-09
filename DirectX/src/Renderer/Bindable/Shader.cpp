#include "pch.h"
#include "Shader.h"

#include <d3dcompiler.h>

Shader::Shader(Graphics* gfx, const std::wstring_view& filepath)
	: Bindable(gfx)
{
	D3DReadFileToBlob(filepath.data(), &m_Blob);
}

Shader::~Shader()
{
	m_Blob->Release();
}

ID3DBlob* Shader::GetBytecode()
{
	return m_Blob;
}

PixelShader::PixelShader(Graphics* gfx, const std::wstring& filepath)
	: Shader(gfx, filepath)
{
	Device()->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_Shader);
}

PixelShader::~PixelShader()
{
	if (m_Shader)
		m_Shader->Release();
}

void PixelShader::Bind()
{
	Context()->PSSetShader(m_Shader, nullptr, 0);
}

VertexShader::VertexShader(Graphics* gfx, const std::wstring& filepath)
	: Shader(gfx, filepath)
{
	Device()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_Shader);
}

VertexShader::~VertexShader()
{
	if (m_Shader)
		m_Shader->Release();
}

void VertexShader::Bind()
{
	Context()->VSSetShader(m_Shader, nullptr, 0);
}
