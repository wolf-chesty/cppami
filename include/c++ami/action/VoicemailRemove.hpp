
#ifndef ACTION_VOICEMAIL_REMOVE_HPP
#define ACTION_VOICEMAIL_REMOVE_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class VoicemailRemove : public Action {
public:
    explicit VoicemailRemove(std::string context, std::string mailbox, std::string folder, std::string id);
};

} // namespace cpp_ami::action

#endif
