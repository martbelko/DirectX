#pragma once

#include "Bindable.h"

class Shader : public Bindable
{
public:
	Shader(Graphics* gfx, const std::wstring_view& filepath);
	virtual ~Shader() override;

	ID3DBlob* GetBytecode();
protected:
	ID3DBlob* m_Blob;
};

class PixelShader : public Shader
{
public:
	PixelShader(Graphics* gfx, const std::wstring& filepath);
	virtual ~PixelShader() override;

	virtual void Bind() override;
protected:
	ID3D11PixelShader* m_Shader;
};

class VertexShader : public Shader
{
public:
	VertexShader(Graphics* gfx, const std::wstring& filepath);
	virtual ~VertexShader() override;

	virtual void Bind() override;
protected:
	ID3D11VertexShader* m_Shader;
};