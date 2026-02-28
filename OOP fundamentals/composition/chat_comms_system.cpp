#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class Message {
private:
    string sender;
    string text;
    time_t timestamp;
public:
    Message(const string& sender, const string& text)
        : sender(sender), text(text), timestamp(time(nullptr)) {}

    void display() const {
        // TODO: Print message in format "[sender]: text"
        cout<<"["<<sender<<"]: "<<text<<endl;
    }

    string getSender() const { return sender; }
    string getText() const { return text; }
};

class Conversation {
private:
    string title;
    vector<Message> messages;
public:
    Conversation(const string& title) : title(title) {}

    void sendMessage(const string& sender, const string& text) {
        // TODO: Create a Message internally and add it to the list
        messages.emplace_back(sender, text);
    }

    void printHistory() const {
        // TODO: Print conversation title and all messages
        cout<<"--- "<<title<<" ---"<<endl;
        for(const auto& message: messages) message.display();
    }

    void deleteConversation() {
        // TODO: Clear all messages (they are destroyed with the conversation)
        messages.clear();
    }

    int getMessageCount() const { return messages.size(); }
    string getTitle() const { return title; }

    void forwardMessage(Conversation& target, int messageIndex) {
        // TODO (Challenge): Copy message content into a NEW Message
        // in the target conversation. Don't move the original.
        target.sendMessage(messages[messageIndex].getSender(), messages[messageIndex].getText());
    }
};

int main() {
    Conversation teamChat("Team Discussion");
    Conversation projectChat("Project Alpha");

    teamChat.sendMessage("Alice", "Hey team, standup in 5 minutes");
    teamChat.sendMessage("Bob", "Got it, joining now");
    teamChat.sendMessage("Alice", "Don't forget to update your tasks");

    projectChat.sendMessage("Charlie", "Deployment is scheduled for Friday");

    cout << "Before deletion:" << endl;
    teamChat.printHistory();
    cout << "Project chat has " << projectChat.getMessageCount() << " messages" << endl << endl;

    // Challenge: forward a message
    teamChat.forwardMessage(projectChat, 2);
    cout << "After forwarding:" << endl;
    projectChat.printHistory();

    // Delete team chat - all its messages are destroyed
    teamChat.deleteConversation();
    cout << "\nAfter deleting team chat:" << endl;
    cout << "Team chat has " << teamChat.getMessageCount() << " messages" << endl;
    cout << "Project chat still has " << projectChat.getMessageCount() << " messages" << endl;

    return 0;
}
