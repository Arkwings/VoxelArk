#pragma once

class GameState {
    public:
    GameState();
    ~GameState();
    void Update(const float delta_time);

    double charac_front_pos_;
    double charac_left_pos_;
    double charac_side_rota_;
    double charac_up_rota_;

    bool position_updated_;
    bool rotation_updated_;
#ifdef DEBUG
    bool debug_print_;
#endif

    unsigned int ticks_;
};