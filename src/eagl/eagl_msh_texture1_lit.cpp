// Partial reconstruction from the pinned GameCube executable.
// See docs/Rendering.md for evidence and remaining compiler differences.
#if defined(DECOMP_COMPARE)
#include "EAGLMaterialLighting.h"

using namespace EAGL;
using namespace EAGLInternal;
static void CompBlock004Moh3_Msh_Texture1(PCodeData &data, StaticData &state) {
    state.state = (GeoPrimState *)data.parameters[8].value;
    float fog = ((float *)data.parameters[10].value)[0];
    bool fogEnabled;
    ContextExtension(Device::Get()->GetCurrentRenderContext())->GetFogEnable(fogEnabled);
    if (fog == 1.0f && !fogEnabled)
        ContextExtension(Device::Get()->GetCurrentRenderContext())->SetFogEnable(true);
    else if (fog == 0.0f && fogEnabled)
        ContextExtension(Device::Get()->GetCurrentRenderContext())->SetFogEnable(false);
    Transform transform = *(Transform *)data.parameters[1].value;
    Transform::Transpose(transform, transform);
    GXLoadPosMtxImm(transform.m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    Transform normals = *(Transform *)data.parameters[1].value;
    Transform::Transpose(normals, normals);
    GXLoadNrmMtxImm(normals.m, GX_PNMTX0);
    unsigned int index = 0;
    unsigned int mask = 0;
    unsigned int bit = 1;
    MaterialLightData *lights = (MaterialLightData *)data.parameters[3].value;
    Transform view = *(Transform *)data.parameters[2].value;
    view.m[3][0] = 0.0f;
    view.m[3][1] = 0.0f;
    view.m[3][2] = 0.0f;
    view.m[3][3] = 1.0f;
    GXSetNumChans(1);
    GXSetChanMatColor(GX_COLOR0A0, WhiteColour());
    GXColor ambient;
    ambient.a = 255;
    ambient.r = (unsigned char)(lights->ambient[0] * 255.0f);
    ambient.g = (unsigned char)(lights->ambient[1] * 255.0f);
    ambient.b = (unsigned char)(lights->ambient[2] * 255.0f);
    GXSetChanAmbColor(GX_COLOR0A0, ambient);
    {
        GXLightObj light;
        COORD4 direction;
        if (lights->colours[index].rgb[0] != 0.0f || lights->colours[index].rgb[1] != 0.0f ||
            lights->colours[index].rgb[2] != 0.0f) {
            const float distance = 1048576.0f;
            GXColor colour;
            colour.a = 255;
            colour.r = (unsigned char)(lights->colours[index].rgb[0] * 255.0f);
            colour.g = (unsigned char)(lights->colours[index].rgb[1] * 255.0f);
            colour.b = (unsigned char)(lights->colours[index].rgb[2] * 255.0f);
            mask |= bit;
            GXInitLightColor(&light, colour);
            view.TransformPoint(lights->directions[index], direction);
            GXInitLightPos(&light, direction.x * distance, direction.y * distance,
                           direction.z * distance);
            GXLoadLightObjImm(&light, (GXLightID)bit);
        }
        ++index;
        bit <<= 1;
        if (lights->colours[index].rgb[0] != 0.0f || lights->colours[index].rgb[1] != 0.0f ||
            lights->colours[index].rgb[2] != 0.0f) {
            const float distance = 1048576.0f;
            GXColor colour;
            colour.a = 255;
            colour.r = (unsigned char)(lights->colours[index].rgb[0] * 255.0f);
            colour.g = (unsigned char)(lights->colours[index].rgb[1] * 255.0f);
            colour.b = (unsigned char)(lights->colours[index].rgb[2] * 255.0f);
            mask |= bit;
            GXInitLightColor(&light, colour);
            view.TransformPoint(lights->directions[index], direction);
            GXInitLightPos(&light, direction.x * distance, direction.y * distance,
                           direction.z * distance);
            GXLoadLightObjImm(&light, (GXLightID)bit);
        }
        ++index;
        bit <<= 1;
        if (lights->colours[index].rgb[0] != 0.0f || lights->colours[index].rgb[1] != 0.0f ||
            lights->colours[index].rgb[2] != 0.0f) {
            const float distance = 1048576.0f;
            GXColor colour;
            colour.a = 255;
            colour.r = (unsigned char)(lights->colours[index].rgb[0] * 255.0f);
            colour.g = (unsigned char)(lights->colours[index].rgb[1] * 255.0f);
            colour.b = (unsigned char)(lights->colours[index].rgb[2] * 255.0f);
            mask |= bit;
            GXInitLightColor(&light, colour);
            view.TransformPoint(lights->directions[index], direction);
            GXInitLightPos(&light, direction.x * distance, direction.y * distance,
                           direction.z * distance);
            GXLoadLightObjImm(&light, (GXLightID)bit);
        }
    }
    GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_VTX, mask, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanCtrl(GX_ALPHA0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    TevStage::SetPS2StyleOverBright(true, false);
    ((GeoPrimStateExtension *)state.state)->Use();
    if (!state.preserveVertexFormat)
        GeoPrimStateExtension::SetCurrentVertex(state.vertexFormat, 3);
    if (data.textureDirty[0])
        ((TAR *)data.parameters[9].value)->Use();
    Transform textureTransform = *(Transform *)data.parameters[12].value;
    Transform::Transpose(textureTransform, textureTransform);
    GXLoadTexMtxImm(textureTransform.m, GX_TEXMTX0, GX_MTX3x4);
    TARExtension::SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_TEXMTX0, 0,
                                 GX_PTIDENTITY);
    TevStage::SetHardware();
    if (!data.drawArrays) {
        CallList(data.displayList, data.displayListSize, data.fastDisplayList);
    } else {
        PrimitiveType primitive;
        state.state->GetPrimitiveType(primitive);
        int count = *(int *)data.parameters[0].value;
        if (gpDisplayList) {
            int batch = (gDisplayListSize - 3) / state.attributesPerVertex;
            batch = batch / 12 * 12;
            GeoPrimStateExtension::SetCurrentVertex(state.vertexFormat, 2);
            if (*(int *)gpDisplayList == -1) {
                *gpDisplayList = (unsigned char)primitive;
                *(unsigned short *)(gpDisplayList + 1) = batch;
                unsigned char *dst = gpDisplayList + 3;
                for (unsigned char i = 0; i < batch; ++i)
                    for (int j = 0; j < state.attributesPerVertex; ++j)
                        *dst++ = i;
                while (dst < gpDisplayList + gDisplayListSize)
                    *dst++ = 0;
                DCStoreRange(gpDisplayList, gDisplayListSize);
            }
            int bytes = (batch * state.attributesPerVertex + 34) / 32 * 32;
            bool called = false;
            while (count >= batch) {
                if (called) {
                    CallList(gpDisplayList, bytes, true);
                } else {
                    GXCallDisplayList(gpDisplayList, bytes);
                    called = true;
                }
                int consumed = batch;
                if (primitive == PRIMITIVE_TRIANGLE_STRIP)
                    consumed -= 2;
                RenderMethod::IncrementArrays(consumed);
                count -= consumed;
            }
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 0);
            FIFO8 = (unsigned char)primitive;
            FIFO16 = count;
            unsigned int i;
            int j;
            if (state.attributesPerVertex == 4) {
                for (i = 0; i < (unsigned)count; ++i)
                    FIFO32 = i | (i << 8) | (i << 16) | (i << 24);
            } else if (state.attributesPerVertex == 3) {
                for (i = 0; i < (unsigned)count; ++i) {
                    FIFO16 = i | (i << 8);
                    FIFO8 = i;
                }
            } else if (state.attributesPerVertex == 2) {
                for (i = 0; i < (unsigned)count; ++i)
                    FIFO16 = i | (i << 8);
            } else if (state.attributesPerVertex == 1) {
                for (i = 0; i < (unsigned)count; ++i)
                    FIFO8 = i;
            } else {
                for (i = 0; i < (unsigned)count; ++i)
                    for (j = 0; j < state.attributesPerVertex; ++j)
                        FIFO8 = i;
            }
            count *= state.attributesPerVertex;
            count += 3;
            while (count & 31) {
                FIFO8 = 0;
                ++count;
            }
        } else {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 0);
            FIFO8 = (unsigned char)primitive;
            FIFO16 = count;
            unsigned int i;
            int j;
            if (state.attributesPerVertex == 4) {
                for (i = 0; i < (unsigned)count; ++i) {
                    FIFO32 = i | (i << 16);
                    FIFO32 = i | (i << 16);
                }
            } else if (state.attributesPerVertex == 3) {
                for (i = 0; i < (unsigned)count; ++i) {
                    FIFO32 = i | (i << 16);
                    FIFO16 = i;
                }
            } else if (state.attributesPerVertex == 2) {
                for (i = 0; i < (unsigned)count; ++i)
                    FIFO32 = i | (i << 16);
            } else if (state.attributesPerVertex == 1) {
                for (i = 0; i < (unsigned)count; ++i)
                    FIFO16 = i;
            } else {
                for (i = 0; i < (unsigned)count; ++i)
                    for (j = 0; j < state.attributesPerVertex; ++j)
                        FIFO16 = i;
            }
            count += count;
            count *= state.attributesPerVertex;
            count += 3;
            while (count & 31) {
                FIFO8 = 0;
                ++count;
            }
        }
    }
}

#endif
