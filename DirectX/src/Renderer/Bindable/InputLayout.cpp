#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(Graphics* gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
	: Bindable(gfx)
{
	Device()->CreateInputLayout(layout.data(), layout.size(), pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(), &m_Layout);
}

InputLayout::~InputLayout()
{
	if (m_Layout)
		m_Layout->Release();
}

void InputLayout::Bind()
{
	Context()->IASetInputLayout(m_Layout);
}
