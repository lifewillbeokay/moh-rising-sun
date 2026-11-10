// CShellMovieSettings: the shell's movie paths from game.cfg; see docs/Frontend.md.
#include "ShellManager.h"
#include <stdio.h>

CShellMovieSettings::CShellMovieSettings() { Reset(); }
CShellMovieSettings::~CShellMovieSettings() {}
void CShellMovieSettings::Reset() {
    fontName[0] = 0;
    backName[0] = 0;
    for (int i = 0; i < 11; i++) {
        paths[0][i].movie[0] = 0;
        paths[0][i].text[0] = 0;
        paths[0][i].time[0] = 0;
        paths[1][i].movie[0] = 0;
        paths[1][i].text[0] = 0;
        paths[1][i].time[0] = 0;
        paths[2][i].movie[0] = 0;
        paths[2][i].text[0] = 0;
        paths[2][i].time[0] = 0;
        paths[3][i].movie[0] = 0;
        paths[3][i].text[0] = 0;
        paths[3][i].time[0] = 0;
    }
    demo.movie[0] = 0;
    demo.text[0] = 0;
    demo.time[0] = 0;
    loaded = 0;
}
void CShellMovieSettings::Load(CConfigFile &config) {
    char name[64];
    sprintf(name, "%s", "game");
    if (config.SetSection(name)) {
        config.AsString("movie_font", fontName, 64);
        config.AsString("movie_back", backName, 64);
        config.AsString("credits_movie", credits.movie, 64);
        config.AsString("credits_text", credits.text, 64);
        config.AsString("credits_time", credits.time, 64);
    }
    for (int i = 0; i <= 10; i++) LoadSection(config, i);
    loaded = 1;
}
void CShellMovieSettings::LoadSection(CConfigFile &config, int index) {
    char name[64];
    sprintf(name, "1_%d", index + 1);
    if (config.SetSection(name)) {
        {
            _ShellMoviePaths *p = &paths[0][index];
            config.AsString("historical_movie", p->movie, 64);
            config.AsString("historical_text", p->text, 64);
            config.AsString("historical_time", p->time, 64);
        }
        {
            _ShellMoviePaths *p = &paths[1][index];
            config.AsString("veteran_movie", p->movie, 64);
            config.AsString("veteran_text", p->text, 64);
            config.AsString("veteran_time", p->time, 64);
        }
        {
            _ShellMoviePaths *p = &paths[2][index];
            config.AsString("anim_movie", p->movie, 64);
            config.AsString("anim_text", p->text, 64);
            config.AsString("anim_time", p->time, 64);
        }
        {
            _ShellMoviePaths *p = &paths[3][index];
            config.AsString("letter_movie", p->movie, 64);
            config.AsString("letter_text", p->text, 64);
            config.AsString("letter_time", p->time, 64);
        }
    }
    sprintf(name, "1_16");
    if (config.SetSection(name)) {
        config.AsString("demo_movie", demo.movie, 64);
        config.AsString("demo_text", demo.text, 64);
        config.AsString("demo_time", demo.time, 64);
    }
}
_ShellMoviePaths *CShellMovieSettings::GetMoviePathsStruct(EMovieType type, int index) { int slot = index - 1;
    return &paths[type][slot]; }
