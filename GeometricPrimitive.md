|[[DirectXTK]]|
|---|

This is a helper for drawing simple geometric shapes including texture coordinates and surface normals.

* Box/Cube
* Cone
* Cylinder
* Dodecahedron
* Icosahedron
* Octahedron
* Sphere (uv or geodesic)
* Teapot (a.k.a. the [Utah teapot](https://wikipedia.org/wiki/Utah_teapot))
* Tetrahedron
* Torus

![GeometricPrimitive Example](https://github.com/Microsoft/DirectXTK/wiki/images/GeometricPrimitiveExample.png)

**Related tutorial:** [[3D shapes]]

# Header
```cpp
#include <GeometricPrimitive.h>
```

# Initialization
The GeometryPrimitive class must be created from a factory method which takes the Direct3D 11 device context.

```cpp
std::unique_ptr<GeometricPrimitive> shape;
shape = GeometricPrimitive::CreateTeapot(deviceContext);
```

For exception safety, the factory functions return a ``std::unique_ptr``.

* **CreateBox**( deviceContext, const XMFLOAT3& size): Creates a box with a non-uniform size.

* **CreateCone**( deviceContext, float diameter = 1, float height = 1, size_t tessellation = 32): Creates a [cone](http://wikipedia.org/wiki/Cone) of a given height, diameter, and tessellation factor.

* **CreateCube**( deviceContext, float size = 1): Creates a [cube](http://wikipedia.org/wiki/Cube) (also known as a [hexahedron](http://wikipedia.org/wiki/Hexahedron)) of the given size.

* **CreateCylinder**( deviceContext, float height = 1, float diameter = 1, size_t tessellation = 32): Creates a [cylinder](http://wikipedia.org/wiki/Cylinder_(geometry)) of given height, diameter, tessellation factor.

* **CreateDodecahedron**( deviceContext, float size = 1): Creates a [dodecahedron](http://wikipedia.org/wiki/Dodecahedron) of a given size.

* **CreateGeoSphere**( deviceContext, float diameter = 1, size_t tessellation = 3): Creates a geodesic [sphere](http://wikipedia.org/wiki/Sphere) with the given diameter and tessellation factor.

* **CreateIcosahedron**( deviceContext, float size = 1): Creates a [icosahedron](http://wikipedia.org/wiki/Icosahedron) of a given size.

* **CreateOctahedron**( deviceContext, float size = 1): Creates a [octahedron](http://wikipedia.org/wiki/Octahedron) of a given size.

* **CreateSphere**( deviceContext, float diameter = 1, size_t tessellation = 16): Creates a uv-[sphere](http://wikipedia.org/wiki/Sphere) of given diameter with the given tessellation factor.

* **CreateTeapot**( deviceContext, float size = 1, size_t tessellation = 8): Creates the [Utah Teapot](http://wikipedia.org/wiki/Utah_teapot) of a given size and tessellation factor.

* **CreateTetrahedron**( deviceContext, float size = 1): Creates a [tetrahedron](http://wikipedia.org/wiki/Tetrahedron) of given size.

* **CreateTorus**( deviceContext, float diameter = 1, float thickness = 0.333f, size_t tessellation = 32): Creates a [torus](http://wikipedia.org/wiki/Torus) of given diameter, thickness, and tessellation factor.

# Type aliases

* **GeometricPrimitive::VertexType** is an alias for [[VertexPositionNormalTexture|VertexTypes]]

* **GeometricPrimitive::VertexCollection** is an alias for ``std::vector<VertexType>``.

* **GeometricPrimitive::IndexCollection** is an alias for ``std::vector<uint16_t>``.

# Simple drawing
Simple solid shape drawing:

```cpp
shape->Draw(world, view, projection, Colors::Green);
```

Simple wireframe shape drawing:

```cpp
shape->Draw(world, view, projection, Colors::Gray, nullptr, true);
```

Simple solid textured shape drawing:

```cpp
shape->Draw(world, view, projection, Colors::Gray, cat);
```

The **Draw** method also accepts an optional callback function which can be used to override the default rendering state:

```cpp
shape->Draw(world, view, projection, Colors::White, catTexture, false, [=]
{
    deviceContext->OMSetBlendState(...);
});
```

This makes use of a [[BasicEffect|Effects]] shared by all geometric primitives drawn on that device context.

# Advanced drawing
This form of drawing makes use of a custom [[IEffect|Effects]] instance. The caller is responsible for using an input layout that matches the effect.

```cpp
IEffect* myeffect = ...

Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
shape->CreateInputLayout( myeffect, inputLayout.GetAddressOf() );

shape->Draw( myeffect, inputLayout.Get() );
```

This takes an optional parameter for enabling alpha-blending, wireframe, and a callback function which can be used to override the default rendering state.

The **Draw** operation will only set up a texture sampler in slot 0. If you are using an [[IEffect|Effects]] instance with more than a single texture, you need to set additional samplers manually (perhaps via ``setCustomState`` callbacks). For example, if using _EnvironmentMapEffect_:

```cpp
shape->Draw( myeffect, inputLayout.Get(), false, false, [=]
{
    ID3D11SamplerState* samplerState[2] = { states.LinearClamp(),
        states.LinearWrap() };
    deviceContext->PSSetSamplers(0, 2, &samplerState);
});
```

> GeometricPrimitive shapes define a single set of texture coordinates, which works well with [[BasicEffect]], [[DebugEffect]],
[[EnvironmentMapEffect]], [[NormalMapEffect]], or [[PBREffect]]. They don't include two sets of texture coordinates, so you can't use them with [[DualTextureEffect]]. They do not include tangents or bi-normals, so they don't work with [[DGSLEffect]]. They don't include skinning weights and bone indices, so they aren't suited for use with [[SkinnedEffect]].

# Coordinate systems
These geometric primitives (based on the XNA Game Studio conventions) use right-handed coordinates. They can be used with left-handed coordinates by setting the _rhcoords_ parameter on the factory methods to 'false' to reverse the winding ordering (the parameter defaults to 'true').

For a left-handed view system:

```cpp
shape = GeometricPrimitive::CreateTeapot( deviceContext, 1.f, 8, false ) );
```

> Using the wrong value for_ rhcoords_ for your viewing setup will result in the objects looking 'inside out'.

# Depth buffer

The rendering setup assumes you are using a standard z-buffer. If have set up your pipeline for [reverse zbuffer](https://developer.nvidia.com/content/depth-precision-visualized) rendering, be sure to set this class property on ``GeometricPrimitive``:

```
GeometricPrimitive::SetDepthBufferMode(true);
```

# Inside vs. Outside
These geometric primitives are intended for view from the 'outside' for efficient back-face culling. However, both spheres and boxes are commonly used to form 'skyboxes' for backgrounds. To support this, you set the _rhcoords_ parameter backwards for your view coordinates, and then set _invertn_ to true.

For a right-handed view system:

```cpp
sky = GeometricPrimitive::CreateBox( deviceContext, XMFLOAT3(10,10,10), false, true);

sky = GeometricPrimitive::CreateSphere( deviceContext, 100.f, false, true);
```

For a left-handed view system:

```cpp
sky = GeometricPrimitive::CreateBox( deviceContext, XMFLOAT3(10,10,10), true, true);

sky = GeometricPrimitive::CreateSphere( deviceContext, 100.f, true, true);
```

# Alpha blending
Alpha blending defaults to using premultiplied alpha. To make use of 'straight' alpha textures, override the blending mode via the optional callback:

```cpp
m_states = std::make_unique<CommonStates>(device);

shape->Draw(world, view, projection, Colors::White, catTexture, false, [=]
{
    deviceContext->OMSetBlendState( m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
});
```

# Custom geometry

There are equivalent static methods for each of the factory methods that return the vertex and index buffer data as ``std::vector``. These values can be modified, and then used to create a customized geometric primitive or drawn through some other mechanism.

```cpp
GeometricPrimitive::VertexCollection vertices;
GeometricPrimitive::IndexCollection indices;
GeometricPrimitive::CreateBox( vertices, indices,
    XMFLOAT3(1.f/2.f, 2.f/2.f, 3.f/2.f) );

// Tile the texture in a 5x5 grid
for(auto& it : vertices)
{
    it.textureCoordinate.x *= 5.f;
    it.textureCoordinate.y *= 5.f;
}

customBox = GeometricPrimitive::CreateCustom( deviceContext, vertices, indices ) );
```

> You can also use this 'two-stage' creation of the geometric primitive to compute a bounding volume from ``vertices``, although for many geometric primitives (i.e. sphere, box, etc.) you can directly create the bounding volume from the same parameters

# Instancing

If you create a [[NormalMapEffect]], [[PBREffect]], or [[DebugEffect]] effect that supports GPU instancing, you can use the following input layout in combination with the **DrawInstanced** method:

```cpp
static const D3D11_INPUT_ELEMENT_DESC s_InputElements[] =
{
    // GeometricPrimitive::VertexType
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
    // XMFLOAT3X4
    { "InstMatrix",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "InstMatrix",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "InstMatrix",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};
```

You need to create a custom input layout object:

```
CreateInputLayoutFromEffect(device, effect,
    s_InputElements, std::size(s_InputElements),
    &instancedIL);
```

Create the per-instance vertex buffer:

```cpp
XMFLOAT3X4 s_instanceTransforms[c_instanceCount] = { ... };

D3D11_BUFFER_DESC desc = {};
desc.ByteWidth = static_cast<UINT>(c_instanceCount * sizeof(XMFLOAT3X4));
desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
desc.Usage = D3D11_USAGE_DYNAMIC;
desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

D3D11_SUBRESOURCE_DATA initData = { s_instanceTransforms, 0, 0 };

DX::ThrowIfFailed(
    device->CreateBuffer(&desc, &initData, &instancedVB)
);
```

You must explicitly bind the per-instance data in a second Vertex Buffer before drawing:

```cpp
UINT stride = sizeof(XMFLOAT3X4);
UINT offset = 0;
context->IASetVertexBuffers(1, 1, instancedVB.GetAddressOf(), &stride, &offset);

...

shape->DrawInstanced(effect, instancedIL.Get(), c_instanceCount);
```

# Custom vertex format

If you want to create a vertex format other than ``VertexPositionNormalTexture``, you can use the ``GeometricPrimitive`` custom geometry methods to generate the shape data, but you'll need to implement the creation of the VB/IB and rendering in your own code (i.e. the ``GeometricPrimitive::CreateCustom`` method only supports ``VertexPositionNormalTexture``).

Here's an example that repurposes the generated normal information into a per-vertex color.

```cpp
Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

UINT indexCount;

std::unique_ptr<DirectX::CommonStates> states;
```

```cpp
// Create shape data
GeometricPrimitive::VertexCollection vertices;
GeometricPrimitive::IndexCollection indices;
GeometricPrimitive::CreateSphere(vertices, indices);

GeometricPrimitive::VertexCollection newVerts;
newVerts.reserve(vertices.size());
for (auto it : vertices)
{
    VertexPositionColor v;
    v.position = it.position;
    v.color = XMFLOAT4(it.normal.x, it.normal.y, it.normal.z, 1.f);
    newVerts.emplace_back(v);
}

// Create VB/IB
auto vsize = UINT(newVerts.size() * sizeof(VertexPositionColor));
auto isize = UINT(indices.size() * sizeof(uint16_t));

auto vdesc = CD3D11_BUFFER_DESC(vsize, D3D11_BIND_VERTEX_BUFFER);
auto idesc = CD3D11_BUFFER_DESC(isize, D3D11_BIND_INDEX_BUFFER);

D3D11_SUBRESOURCE_DATA initData = { newVerts.data(), vsize, 0 };
DX::ThrowIfFailed(device->CreateBuffer(&vdesc, &initData,
    vertexBuffer.ReleaseAndGetAddressOf()));

initData = { indices.data(), isize, 0 };
DX::ThrowIfFailed(device->CreateBuffer(&idesc, &initData,
    indexBuffer.ReleaseAndGetAddressOf()));

// Create matching effect & layout for our vertex
effect = std::make_unique<BasicEffect>(device);
effect->SetVertexColorEnabled(true);

void const* shaderByteCode;
size_t byteCodeLength;

effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

device->CreateInputLayout(
    VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
    shaderByteCode, byteCodeLength, inputLayout.ReleaseAndGetAddressOf());

indexCount = UINT(indices.size());

states = std::make_unique<CommonStates>(device);
```

```cpp
// Render using our effect
deviceContext->OMSetBlendState( states->Opaque(), nullptr, 0xFFFFFFFF );
deviceContext->OMSetDepthStencilState( states->DepthNone(), 0 );
deviceContext->RSSetState( states->CullNone() );

effect->Apply(deviceContext);
deviceContext->IASetInputLayout(inputLayout.Get());

auto vb = vertexBuffer.Get();
UINT vertexStride = sizeof(VertexPositionColor);
UINT vertexOffset = 0;

deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexStride, &vertexOffset);
deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

deviceContext->DrawIndexed(indexCount, 0, 0);
```

# Feature Level Notes
In order to support [all feature levels](https://docs.microsoft.com/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro), the GeometricPrimitive implementation make use of 16-bit indices (``DXGI_FORMAT_R16_UINT``) which limits to a maximum of 65535 vertices.

Feature Level 9.1 is also limited to a maximum of 65535 primitives in a single draw. This rules out using extremely large numbers for the tessellation factor, although the exact limit depends on the shape implementation.

Note that these shapes tend to duplicate rather than share vertices in order to provide the 'face-normal' lighting expected of sharp geometry.

# Threading model
Each GeometricPrimitive instance only supports drawing from one thread at a time, but you can simultaneously submit primitives on multiple threads if you create a separate GeometricPrimitive instance per Direct3D 11 deferred context.

[Immediate and Deferred Rendering](https://docs.microsoft.com/windows/win32/direct3d11/overviews-direct3d-11-render-multi-thread-render)

# State management

When ``Draw`` is called,  it will set the states needed to render with the effect. Existing state is not save or restored. For efficiency, it simply sets the state it requires to render and assumes that any subsequent rendering will overwrite state that it needs.

GeometricPrimitive makes use of the following states:

* BlendState
* Constant buffer (Vertex Shader and Pixel Shader stages, slot 0)
* DepthStencilState
* Index buffer
* Input layout
* Pixel shader
* Primitive topology
* RasterizerState
* SamplerState (Pixel Shader stage, slot 0)
* Shader resources (Pixel Shader stage, slot 0)
* Vertex buffer (slot 0)
* Vertex shader

The GeometricPrimitive class assumes you've already set the Render Target view, Depth Stencil view, and Viewport.

# Remark
Tetrahedron, Cube/Hexahedron, Octahedron, Dodecahedron, and Icosahedron comprise the five [Platonic solids](https://wikipedia.org/wiki/Platonic_solid). The surface normals for these shapes are constructed as "face-normals" for faceted shading.

The *Utah Teapot* (also known as the *Newell Teapot*) is sometimes jokingly referred to as the "Sixth Platonic solid" due to its prevalence in rendering sample images. It was created in 1975 by Martin Newell at the University of Utah. It's become the "Hello, world" of 3D models hence why it's included as a basic geometric primitive shape in *DirectX Tool Kit*.
