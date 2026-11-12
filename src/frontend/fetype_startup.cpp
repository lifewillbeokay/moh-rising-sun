// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#include "FEngine.h"
#include "FETypeNode.h"
#include "FEngInterpolation.h"
extern const char *FEColor1Name, *FEColor2Name, *FEColor3Name, *FEColor4Name, *FEFrameNumName;
int FETypeLib::Startup() {
    FETypeNode *type = CreateImageObjectType("Image");
    type->type = 1; types.AddNode(types.tail, type);
    type = CreateImageObjectType("CBV Image");
    type->AddField(FEColor1Name, 6); type->AddField(FEColor2Name, 6);
    type->AddField(FEColor3Name, 6); type->AddField(FEColor4Name, 6);
    FEColor white; white = FEColor(0xffffffff);
    type->GetField(FEColor1Name)->SetDefault(&white); type->GetField(FEColor2Name)->SetDefault(&white);
    type->GetField(FEColor3Name)->SetDefault(&white); type->GetField(FEColor4Name)->SetDefault(&white);
    type->type = 9; types.AddNode(types.tail, type);
    type = CreateImageObjectType("Anim Image");
    type->AddField(FEFrameNumName, 1);
    long frame = 0; type->GetField(FEFrameNumName)->SetDefault(&frame);
    type->type = 10; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("Simple Image"); type->type = 11; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("String"); type->type = 2; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("Model"); type->type = 3; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("Movie"); type->type = 7; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("Effect"); type->type = 8; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("List"); type->type = 4; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("Group"); type->type = 5; types.AddNode(types.tail, type);
    type = CreateBaseObjectType("Code List"); type->type = 6; types.AddNode(types.tail, type);
    return 1;
}
