cd src\shaders

glslc plane.vert -o planeVert.spv
glslc plane.frag -o planeFrag.spv
glslc vertex.vert -o vert.spv
glslc fragment.frag -o frag.spv
glslc particleShader.vert -o particleShaderVert.spv
glslc particleShader.frag -o particleShaderFrag.spv
glslc bulletVert.vert -o bulletVert.spv
glslc bulletFrag.frag -o bulletFrag.spv
glslc planeLightsVert.vert -o planeLightsVert.spv
glslc planeLightsFrag.frag -o planeLightsFrag.spv