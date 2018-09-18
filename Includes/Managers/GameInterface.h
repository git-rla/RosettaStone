#ifndef HEARTHSTONEPP_INTERFACE_H
#define HEARTHSTONEPP_INTERFACE_H

#include <Enums/TaskEnums.h>
#include <Managers/GameAgent.h>
#include <Tasks/TaskMeta.h>

#include <Flatbuffers/generated/FlatData_generated.h>

#include <array>
#include <functional>
#include <iostream>
#include <map>

namespace Hearthstonepp
{
using EntityVector = flatbuffers::Vector<flatbuffers::Offset<FlatData::Entity>>;

//!
//! \brief GameResult structure.
//!
//! This structure stores the result of Hearthstone game.
//!
struct GameResult
{
    std::string winnerID;
};

//! \brief An enumerator for identifying handle status.
enum class HandleStatus
{
    CONTINUE = 0,  //!< Continues the game.
    STOP = 1,      //!< Ends the game.
};

//!
//! \brief GameInterface class.
//!
//! This class is a interface for Hearthstone game agent.
//! Also, it handles various messages and tasks. All tasks are logged.
//!
class GameInterface
{
 public:
    //! Constructs game interface with given \p agent, \p output and \p input.
    //! \param agent The agent for the game.
    //! \param output The output stream. (std::cout as default)
    //! \param input The input stream. (std::cin as default)
    GameInterface(GameAgent& agent, std::ostream& output = std::cout,
                  std::istream& input = std::cin);

    //! Starts the game. And over and over again, until it's done.
    //! \return The result of the game.
    GameResult StartGame();

 private:
    //! Handles various messages.
    //! \param meta Serialized task meta.
    //! \return An enumerator that indicates the status of the game.
    HandleStatus HandleMessage(const TaskMeta& meta);

    //! Writes log.
    //! \param name The name to write to log (rvalue ref).
    //! \return The output stream.
    std::ostream& LogWriter(std::string&& name);

    //! Writes log.
    //! \param name The name to write to log (const lvalue ref).
    //! \return The output stream.
    std::ostream& LogWriter(const std::string& name);

    //! Shows game menus.
    //! \tparam SIZE The number of game menus.
    //! \params menus An array in which game menus are stored.
    template <std::size_t SIZE>
    void ShowMenus(const std::array<std::string, SIZE>& menus);

    //! Shows cards.
    //! \param entities An array in which cards are stored.
    void ShowCards(const EntityVector& entities);

    // MARK: TaskMeta Handler

    //! Handles default task.
    //! \param meta Serialized task meta.
    void HandleDefault(const TaskMeta& meta);

    //! Handles task vector.
    //! \param meta Serialized task meta.
    void HandleTaskVector(const TaskMeta& meta);

    //! Handles player setting.
    //! \param meta Serialized task meta.
    void HandlePlayerSetting(const TaskMeta& meta);

    //! Handles requirement.
    //! \param meta Serialized task meta.
    void HandleRequire(const TaskMeta& meta);

    //! Handles game briefing.
    //! \param meta Serialized task meta.
    void HandleBrief(const TaskMeta& meta);

    //! Handles game over.
    //! \param meta Serialized task meta.
    void HandleGameEnd(const TaskMeta& meta);

    //! Handles over draw.
    //! \param meta Serialized task meta.
    void HandleOverDraw(const TaskMeta& meta);

    // MARK: Input Task Handler

    //! Inputs mulligan. The mulligan, or card selection stage, occurs at the
    //! start of each match. Each player is shown their randomly selected
    //! starting hand and is given the option to redraw as many of those cards
    //! as they like.
    //! \param meta Serialized task meta.
    void InputMulligan(const TaskMeta& meta);

    //! Inputs the value you selected in the game menu.
    //! \param meta Serialized task meta.
    void InputMenu(const TaskMeta& meta);

    //! Inputs the card to choose.
    //! \param meta Serialized task meta.
    void InputCard(const TaskMeta& meta);

    //! Inputs the target.
    //! \param meta Serialized task meta.
    void InputTarget(const TaskMeta& meta);

    //! Inputs the position.
    //! \param meta Serialized task meta.
    void InputPosition(const TaskMeta& meta);

    std::array<std::string, GAME_MAIN_MENU_SIZE> m_mainMenuStr = {
        "1. Use Card", "2. Combat", "3. Stop"
    };

    // Handler table
    std::map<TaskID, std::function<void(GameInterface&, const TaskMeta&)>>
        m_handler = {
            { TaskID::TASK_VECTOR, &GameInterface::HandleTaskVector },
            { TaskID::PLAYER_SETTING, &GameInterface::HandlePlayerSetting },
            { TaskID::REQUIRE, &GameInterface::HandleRequire },
            { TaskID::BRIEF, &GameInterface::HandleBrief },
            { TaskID::GAME_END, &GameInterface::HandleGameEnd },
            { TaskID::OVER_DRAW, &GameInterface::HandleOverDraw },
        };

    // Input Handler Table
    std::map<TaskID, std::function<void(GameInterface&, const TaskMeta&)>>
        m_inputHandler = {
            { TaskID::MULLIGAN, &GameInterface::InputMulligan },
            { TaskID::SELECT_MENU, &GameInterface::InputMenu },
            { TaskID::SELECT_CARD, &GameInterface::InputCard },
            { TaskID::SELECT_TARGET, &GameInterface::InputTarget },
            { TaskID::SELECT_POSITION, &GameInterface::InputPosition },
        };

    GameAgent& m_agent;
    TaskMeta m_buffer;
    GameResult m_result;

    std::ostream& m_ostream;
    std::istream& m_istream;

    std::string m_users[2];
    //! Cached game status for processing some requirements.
    std::unique_ptr<BYTE[]> m_status;
};
}  // namespace Hearthstonepp

#endif
