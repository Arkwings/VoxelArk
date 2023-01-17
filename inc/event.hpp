#pragma once

class GameState;

class Event {
    public:
    Event();
    ~Event();
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    void Process(GameState& gs);

    private:
    void processKeyboard(GameState& gs);
    void processMouse(GameState& gs);

    std::vector<int> keys_;
};
