// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_ADAPTIVE_EFFECT_HPP
#define ROSETTASTONE_ADAPTIVE_EFFECT_HPP

#include <Rosetta/Auras/Aura.hpp>

namespace RosettaStone
{
//!
//! \brief AdaptiveEffect class.
//!
//! Effects of this kind of Auras are influenced by other factors in game, in
//! real time. e.g. Lightspawn, Southsea Deckhand.
//!
class AdaptiveEffect : public Aura
{
 public:
    //! Constructs adaptive effect with given \p _condition and \p tags.
    //! \param _condition The specific condition.
    //! \param tags The given tags.
    AdaptiveEffect(SelfCondition* _condition, std::vector<GameTag> tags);

    //! Create new Aura instance to the owner's game.
    //! \param owner An owner of adaptive effect.
    //! \param cloning The flag to indicate that it is cloned.
    void Activate(Entity* owner, bool cloning = false) override;

    //! Updates this effect to apply the effect to recently modified entities.
    void Update() override;

    //! Removes this effect from the game to stop affecting entities.
    void Remove() override;

    //! Clones aura effect to \p clone.
    //! \param clone The entity to clone aura effect.
    void Clone(Entity* clone) override;

 private:
    //! Constructs adaptive effect with given \p prototype and \p owner.
    //! \param prototype An adaptive effect for prototype.
    //! \param owner An owner of adaptive effect.
    AdaptiveEffect(AdaptiveEffect& prototype, Entity& owner);

    std::vector<GameTag> m_tags;
    std::vector<int> m_lastValues;

    bool m_isSwitching = false;
};
}  // namespace RosettaStone

#endif  // ROSETTASTONE_ADAPTIVE_EFFECT_HPP
