// A fragment of Rising Sun's AI code: a breadth-first flood through the area path nodes around a start
// area. Class and method names come from the symbol table; member names are inferred from this code.
class CFloodSearch {
public:
    void AddArea(short area);
    short RemoveArea();
    void AddConnectedAreas(short area);
    void ExpandList();
    void BeginSearch(short area, float radius);
    int GetNextArea(short &area);

    short list[32];
    int head;
    int tail;
    int expanded;
    int pending;
    int total;
    unsigned int visited[16];
    float radiusSquared;
    int limit;
};

void CFloodSearch::ExpandList() {
    int end = tail;
    for (int i = expanded; i < end; i++)
        AddConnectedAreas(list[i]);
    expanded = head;
}

void CFloodSearch::BeginSearch(short area, float radius) {
    head = 0;
    tail = 0;
    expanded = 0;
    for (int i = 15; i >= 0; i--)
        visited[i] = 0;
    radiusSquared = radius * radius;
    limit = 32;
    pending = 0;
    total = 0;
    AddArea(area);
}

int CFloodSearch::GetNextArea(short &area) {
    if (head == tail)
        ExpandList();
    if (head != tail) {
        area = RemoveArea();
        return 1;
    }
    return 0;
}
