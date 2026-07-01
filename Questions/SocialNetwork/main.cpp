#include <atomic>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

enum class PrivacyStatus {
  PUBLIC,
  PRIVATE,
};

class Profile {
private:
  string profilePicUrl;
  string bio;
  vector<string> interests;
  PrivacyStatus privacyStatus;

public:
  Profile(const string &profilePicUrl, const string &bio,
          vector<string> &interests, const PrivacyStatus privacyStatus)
      : profilePicUrl(profilePicUrl), bio(bio), interests(interests),
        privacyStatus(privacyStatus) {}

  string getProfilePic() const { return profilePicUrl; }
  string getBio() const { return bio; }
  vector<string> getInterests() const { return interests; }
  PrivacyStatus getPrivacyStatus() const { return privacyStatus; }
};

class User {
private:
  string id;
  string name;
  string email;
  Profile profile;

public:
  User(const string &id, const string &name, const string &email,
       Profile profile)
      : id(id), name(name), email(email), profile(profile) {}

  string getId() const { return id; }

  string getName() const { return name; }

  string getEmail() const { return email; }

  string getProfilePic() const { return profile.getProfilePic(); }

  string getBio() const { return profile.getBio(); }

  vector<string> getInterests() const { return profile.getInterests(); }

  PrivacyStatus getPrivacyStatus() const { return profile.getPrivacyStatus(); }
};

class UserService {
private:
  unordered_map<string, shared_ptr<User>> users;

public:
  shared_ptr<User> getUser(string id) { return users[id]; }

  shared_ptr<User> registerUser(const string &id, const string &name,
                                const string &email,
                                const string &profilePicUrl, const string &bio,
                                vector<string> &interests,
                                PrivacyStatus privacyStatus) {
    auto user = make_shared<User>(
        id, name, email, Profile{profilePicUrl, bio, interests, privacyStatus});
    users[user->getId()] = user;
    return user;
  }
};

enum class FriendRequestStatus {
  PENDING,
  ACCEPTED,
  REJECTED,
};

class FriendRequest {
private:
  string id;
  shared_ptr<User> sender;
  shared_ptr<User> receiver;
  FriendRequestStatus status;

public:
  FriendRequest(const string id, shared_ptr<User> sender,
                shared_ptr<User> receiver)
      : id(id), sender(sender), receiver(receiver),
        status(FriendRequestStatus::PENDING) {}

  FriendRequestStatus getStatus() const { return status; }

  void setStatus(FriendRequestStatus status) { this->status = status; }

  shared_ptr<User> getSender() const { return sender; }
  shared_ptr<User> getReceiver() const { return receiver; }
};

class FriendService {
private:
  unordered_map<string, vector<string>> friends;
  unordered_map<string, FriendRequest *> requests;

public:
  string sendRequest(const string &id, shared_ptr<User> sender,
                     shared_ptr<User> receiver) {
    if (isAlreadyFriend(sender, receiver)) {
      cout << "Already Friends" << endl;
      throw runtime_error("Already friends");
    }
    auto request = new FriendRequest(id, sender, receiver);
    requests[id] = request;
    return id;
  }

  void acceptRequest(const string &requestId) {
    auto request = requests[requestId];

    if (request->getStatus() == FriendRequestStatus::ACCEPTED) {
      cout << "Already Friends" << endl;
      return;
    }

    request->setStatus(FriendRequestStatus::ACCEPTED);
    friends[request->getSender()->getId()].push_back(
        request->getReceiver()->getId());
    friends[request->getReceiver()->getId()].push_back(
        request->getSender()->getId());
  }

  void rejectRequest(const string &requestId) {
    auto request = requests[requestId];

    if (request->getStatus() == FriendRequestStatus::ACCEPTED) {
      cout << "Already Friends" << endl;
      return;
    }
    request->setStatus(FriendRequestStatus::REJECTED);
  }

  vector<string> getFriends(const string &userid) { return friends[userid]; }

private:
  bool isAlreadyFriend(shared_ptr<User> sender, shared_ptr<User> receiver) {
    for (auto id : friends[sender->getBio()]) {
      if (id == receiver->getId()) {
        return true;
      }
    }
    return false;
  }
};

class Comment {
private:
  string id;
  string content;
  shared_ptr<User> user;

public:
  Comment(const string &id, const string &content, shared_ptr<User> user)
      : id(id), content(content), user(user) {}

  string getContent() const { return content; }
  shared_ptr<User> getUser() { return user; }
};

class Like {
private:
  string id;
  shared_ptr<User> user;

public:
  Like(const string &id, shared_ptr<User> user) : id(id), user(user) {}

  shared_ptr<User> getUser() { return user; }
};

class Post {
private:
  string id;
  string content;
  string mediaUrl;
  vector<Like> likes;
  vector<Comment> comments;
  shared_ptr<User> poster;

  atomic<int> likeCount;
  atomic<int> commentCount;

public:
  Post(const string &id, const string &content, const string &mediaUrl,
       shared_ptr<User> poster)
      : id(id), content(content), mediaUrl(mediaUrl), poster(poster) {}

  string getId() const { return id; }
  string getContent() const { return content; }
  string getMediaUrl() const { return mediaUrl; }

  vector<Comment> getComments() const { return comments; }
  int getLikeCount() const { return likeCount; }

  shared_ptr<User> getPoster() { return poster; }

  void addLike(shared_ptr<User> user) {
    likes.push_back(
        (Like(id + "_L_" + to_string(likeCount.fetch_add(1)), user)));
  }

  void addComment(shared_ptr<User> user, const string &content) {
    comments.push_back(Comment(
        id + "_P_" + to_string(commentCount.fetch_add(1)), content, user));
  }

  void renderPost() {
    cout << "mediaUrl: " << mediaUrl << endl;
    cout << "Content: " << content << endl;
    cout << "Like Count: " << likeCount << endl;
  }
};

class PostService {
private:
  unordered_map<string, shared_ptr<Post>> postsMap;
  vector<string> posts;

public:
  string createPost(const string &id, const string &content,
                    const string &mediaUrl, shared_ptr<User> poster) {
    postsMap[id] = make_shared<Post>(id, content, mediaUrl, poster);
    posts.push_back(id);
    return id;
  }

  void likePost(const string &postid, shared_ptr<User> liker) {
    if (!postsMap.count(postid)) {
      cout << "Post not available" << endl;
      return;
    }

    postsMap[postid]->addLike(liker);
  }

  void addComment(const string &postid, shared_ptr<User> user,
                  const string &content) {
    if (!postsMap.count(postid)) {
      cout << "Post not available" << endl;
      return;
    }
    postsMap[postid]->addComment(user, content);
  }

  vector<shared_ptr<Post>> getPosts(unordered_set<shared_ptr<User>> posters) {
    vector<shared_ptr<Post>> finalPosts;

    for (auto it = posts.rbegin(); it != posts.rend(); it++) {
      auto poster = postsMap[*it]->getPoster();
      if (posters.count(poster)) {
        finalPosts.push_back(postsMap[*it]);
      }
    }

    return finalPosts;
  }

  void renderPosts(vector<shared_ptr<Post>> posts) {
    for (auto post : posts) {
      cout << "===================" << endl;
      post->renderPost();
      cout << "===================" << endl;
    }
  }
};

class FeedService {
private:
  shared_ptr<PostService> postService;
  shared_ptr<FriendService> friendService;
  shared_ptr<UserService> userService;

public:
  FeedService(shared_ptr<PostService> postService,
              shared_ptr<FriendService> friendService,
              shared_ptr<UserService> userService)
      : postService(postService), friendService(friendService),
        userService(userService) {}

  vector<shared_ptr<Post>> getFeed(const string &userid) {
    auto user = userService->getUser(userid);
    auto friendsIds = friendService->getFriends(userid);
    unordered_set<shared_ptr<User>> friends;
    for (auto &friendId : friendsIds) {
      friends.insert(userService->getUser(friendId));
    }
    auto posts = postService->getPosts(friends);

    return posts;
  }
};

class Notification;

int main() {
  auto userService = make_shared<UserService>();
  auto postService = make_shared<PostService>();
  auto friendService = make_shared<FriendService>();
  FeedService feedService(postService, friendService, userService);

  vector<string> interests1 = {};
  vector<string> interests2 = {};
  auto u1 = userService->registerUser("U1", "Anurag", "A@b.c", "Monkey", "Live",
                                      interests1, PrivacyStatus::PUBLIC);
  auto u2 =
      userService->registerUser("U2", "Mark", "M@b.c", "Banana", "Don't cramp",
                                interests2, PrivacyStatus::PUBLIC);

  auto requestId1 = friendService->sendRequest("R1", u1, u2);

  friendService->acceptRequest(requestId1);

  auto p1 = postService->createPost("P1", "Hello there", "image.png", u1);
  auto p2 = postService->createPost("P2", "second post", "image2.png", u1);
  auto p3 = postService->createPost("P3", "new here", "my.jpg", u2);

  postService->likePost(p3, u1);

  auto feedPosts = feedService.getFeed("U1");
  postService->renderPosts(feedPosts);

  auto feedPost2 = feedService.getFeed("U2");
  postService->renderPosts(feedPost2);

  return 0;
}
