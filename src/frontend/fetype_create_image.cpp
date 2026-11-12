// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FETypeNode.h"
#include "FEngInterpolation.h"
FETypeNode *FETypeLib::CreateImageObjectType(const char *name) {
    FEVector2 zero = {0, 0}, one = {0, 0};
    FETypeNode *type = CreateBaseObjectType(name);
    type->AddField("Upper Left", 3); type->AddField("Lower Right", 3);
    zero.x = zero.y = 0;
    FEFieldNode *field = type->GetField("Upper Left");
    field->SetDefault(&zero);
    one.x = one.y = 1;
    ((FEFieldNode *)field->next)->SetDefault(&one);
    return type;
}
#endif
