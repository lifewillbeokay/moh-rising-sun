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

static inline bool HasBit(unsigned int word, unsigned int bit) { return (word & bit) != 0; }

void CFloodSearch::AddArea(short area) {
    unsigned int word = area >> 5;
    unsigned int bit = 1 << (area & 31);
    if (!HasBit(visited[word], bit)) {
        list[tail++] = area;
        visited[word] |= bit;
        total++;
        pending++;
    }
}

short CFloodSearch::RemoveArea() {
    pending--;
    return list[head++];
}

