#include "common.hpp"
#include "camera.hpp"
#include "gamestate.hpp"

//gs i created after everything, it should be able to do each thing once at init

GameState::GameState()
    : charac_front_pos_(0)
    , charac_left_pos_(0)
    , charac_side_rota_(0.0f)
    , charac_up_rota_(G_Window->GetHeight() / 2.0f)
    , ticks_(0U)
#ifdef DEBUG
    , debug_print_(false)
#endif
{
    G_CameraHandler->GetActive()->SetLooking();
    G_NoiseMap->LoadChunks(G_CameraHandler->GetActive()->getPos());
    G_NoiseMap->SetChunkOnCamera();
}

GameState::~GameState() {}

void GameState::Update(const float delta_time) {
    G_CameraHandler->GetActive()->Update(charac_side_rota_, charac_up_rota_, static_cast<int>(charac_front_pos_), static_cast<int>(charac_left_pos_), delta_time);
    charac_front_pos_ = 0;
    charac_left_pos_ = 0;

    ticks_ = ticks_ + 1U;
    if (ticks_ > OPT::TICKS_BY_DAY)   ticks_ -= OPT::TICKS_BY_DAY;
    G_Sky->UpdateTransfo(ticks_);

    G_CameraHandler->GetActive()->SetLooking();

    // if (position_updated_) {
    G_NoiseMap->LoadChunks(G_CameraHandler->GetActive()->getPos());
    // }

    G_NoiseMap->Load();
    G_NoiseMap->SetChunkOnCamera();
}