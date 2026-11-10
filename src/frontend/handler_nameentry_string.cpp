// HandlerNameEntry, the name entry screen: its buffer; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "Shell.h"
#include <stdio.h>
#include <string.h>
char *HandlerNameEntry::GetMyString() { return text; }
int HandlerNameEntry::GetStringLength() { return 9; }
