#include <Mage/Mage.h>

import Game;

namespace {
int go() {
  LOG_INFO("Game Starting");

  Game game;
  game.run();

  LOG_INFO("Game Ending");
  return 0;
}
} // namespace

int main(int argc, char *argv[]) { return go(); }

#ifdef _WIN32
#include <Windows.h>
// ReSharper disable CppInconsistentNaming
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                   _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
  return go();
}
#endif
