#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

class Company;
class User;
class JobPosting;

// Forward declarations and existing classes remain the same
class User {
private:
  string id;
  string name;
  string email;
  string password;
  string headline;
  string summary;
  string experience;
  string education;
  vector<string> skills;
  bool isRecruiter;
  weak_ptr<Company> company;

public:
  User(const string id, const string &name, const string &email,
       const string &password, const string &headline, const string &summary,
       const string &experience, const string &education, vector<string> skills,
       const bool isRecruiter, weak_ptr<Company> company)
      : id(id), name(name), email(email), password(password),
        headline(headline), summary(summary), experience(experience),
        education(education), skills(skills), isRecruiter(isRecruiter),
        company(company) {}

  void updateProfile() {}

  void changeHeadline(const string &newHeadline) { headline = newHeadline; }

  void addSkill(const string &skill) {
    for (const auto &s : skills) {
      if (skill == s)
        return;
    }
    skills.push_back(skill);
  }

  void removeSkill(const string &skill) {
    skills.erase(remove(skills.begin(), skills.end(), skill), skills.end());
  }

  // Getters
  string getId() const { return id; }
  string getName() const { return name; }
  string getEmail() const { return email; }
  string getHeadline() const { return headline; }
  vector<string> getSkills() const { return skills; }
  bool getIsRecruiter() const { return isRecruiter; }
};

class Company {
private:
  string id;
  string name;
  string description;
  string industry;
  string location;
  vector<shared_ptr<User>> recruiters;
  vector<shared_ptr<User>> employees;
  vector<shared_ptr<JobPosting>> jobPostings;

public:
  Company(const string &id, const string &name, const string &description,
          const string &industry, const string &location)
      : id(id), name(name), description(description), industry(industry),
        location(location) {}

  void postJob(shared_ptr<JobPosting> jobPost) {
    jobPostings.push_back(jobPost);
  }

  void addRecruiter(shared_ptr<User> recruiter) {
    for (auto r : recruiters) {
      if (r == recruiter)
        return;
    }
    recruiters.push_back(recruiter);
  }

  void removeRecruiter(shared_ptr<User> recruiter) {
    recruiters.erase(remove(recruiters.begin(), recruiters.end(), recruiter),
                     recruiters.end());
  }

  // Getters
  string getId() const { return id; }
  string getName() const { return name; }
  vector<shared_ptr<JobPosting>> getJobPostings() const { return jobPostings; }
};

enum class ConnectionRequestStatus { ACCEPTED, PENDING, DECLINED };

class ConnectionRequest {
private:
  string id;
  string senderId;
  string receiverId;
  ConnectionRequestStatus status;

public:
  ConnectionRequest(const string &id, const string &senderId,
                    const string &receiverId)
      : id(id), senderId(senderId), receiverId(receiverId),
        status(ConnectionRequestStatus::PENDING) {}

  void accept() { status = ConnectionRequestStatus::ACCEPTED; }

  void reject() { status = ConnectionRequestStatus::DECLINED; }

  string getSenderId() const { return senderId; }
  string getReceiverId() const { return receiverId; }
  ConnectionRequestStatus getStatus() const { return status; }
};

class Connection {
private:
  shared_ptr<User> user1;
  shared_ptr<User> user2;

public:
  Connection(shared_ptr<User> user1, shared_ptr<User> user2)
      : user1(user1), user2(user2) {}

  bool hasUser(shared_ptr<User> user) const {
    return user == user1 || user == user2;
  }
};

class Message {
private:
  string id;
  shared_ptr<User> sender;
  string content;
  chrono::system_clock::time_point timestamp;

public:
  Message(const string &id, shared_ptr<User> sender, const string &content)
      : id(id), sender(sender), content(content),
        timestamp(chrono::system_clock::now()) {}

  string getContent() const { return content; }
  shared_ptr<User> getSender() const { return sender; }
};

class Conversation {
private:
  string id;
  vector<shared_ptr<User>> participants;
  vector<shared_ptr<Message>> messages;

public:
  Conversation(const string &id, vector<shared_ptr<User>> participants)
      : id(id), participants(participants) {}

  void sendMessage(shared_ptr<Message> message) { messages.push_back(message); }

  vector<shared_ptr<Message>> getMessages() const { return messages; }
  vector<shared_ptr<User>> getParticipants() const { return participants; }
};

enum class JobPostingStatus { OPEN, CLOSED };

class JobPosting {
private:
  string id;
  string title;
  string description;
  string requirements;
  string location;
  shared_ptr<Company> company;
  JobPostingStatus status;

public:
  JobPosting(const string &id, const string &title, const string &description,
             const string &requirements, const string &location,
             shared_ptr<Company> company)
      : id(id), title(title), description(description),
        requirements(requirements), location(location), company(company),
        status(JobPostingStatus::OPEN) {}

  void close() { status = JobPostingStatus::CLOSED; }

  void reopen() { status = JobPostingStatus::OPEN; }

  // Getters
  string getId() const { return id; }
  string getTitle() const { return title; }
  shared_ptr<Company> getCompany() const { return company; }
  JobPostingStatus getStatus() const { return status; }
};

enum class JobApplicationStatus { APPLIED, REJECTED, SHORTLISTED, ACCEPTED };

class JobApplication {
private:
  string id;
  shared_ptr<User> user;
  shared_ptr<JobPosting> job;
  JobApplicationStatus status;
  chrono::system_clock::time_point appliedDate;

public:
  JobApplication(const string &id, shared_ptr<User> user,
                 shared_ptr<JobPosting> job)
      : id(id), user(user), job(job), status(JobApplicationStatus::APPLIED),
        appliedDate(chrono::system_clock::now()) {}

  void updateStatus(JobApplicationStatus newStatus) { status = newStatus; }

  string getId() const { return id; }
  shared_ptr<User> getUser() const { return user; }
  shared_ptr<JobPosting> getJob() const { return job; }
  JobApplicationStatus getStatus() const { return status; }
};

enum class NotificationType {
  CONNECTION_REQUEST,
  MESSAGE,
  JOB_POSTED,
  JOB_APPLICATION,
};

class Notification {
private:
  string id;
  shared_ptr<User> receiver;
  string message;
  NotificationType type;
  bool isRead;
  chrono::system_clock::time_point timestamp;

public:
  Notification(const string &id, shared_ptr<User> receiver,
               const string &message, const NotificationType &type)
      : id(id), receiver(receiver), message(message), type(type), isRead(false),
        timestamp(chrono::system_clock::now()) {}

  void markAsRead() { isRead = true; }
  string getMessage() const { return message; }
  bool getIsRead() const { return isRead; }
};

class Session {
private:
  string sessionId;
  string userId;
  bool active;
  chrono::system_clock::time_point createdAt;

public:
  Session(const string &sessionId, const string &userId)
      : sessionId(sessionId), userId(userId), active(true),
        createdAt(chrono::system_clock::now()) {}

  void invalidate() { active = false; }
  bool isActive() const { return active; }
  string getUserId() const { return userId; }
};

// Utility class for generating IDs
class IdGenerator {
private:
  static random_device rd;
  static mt19937 gen;
  static uniform_int_distribution<> dis;

public:
  static string generateId() {
    stringstream ss;
    ss << "ID_"
       << chrono::duration_cast<chrono::milliseconds>(
              chrono::system_clock::now().time_since_epoch())
              .count()
       << "_" << dis(gen);
    return ss.str();
  }
};

random_device IdGenerator::rd;
mt19937 IdGenerator::gen(IdGenerator::rd());
uniform_int_distribution<> IdGenerator::dis(1000, 9999);

class AuthService {
private:
  unordered_map<string, shared_ptr<User>> users;
  unordered_map<string, shared_ptr<Session>> sessions;
  unordered_map<string, string> emailToUserId;

public:
  bool registerUser(const string &name, const string &email,
                    const string &password, const string &headline = "",
                    const string &summary = "", const string &experience = "",
                    const string &education = "", vector<string> skills = {},
                    bool isRecruiter = false) {
    // Check if email already exists
    if (emailToUserId.find(email) != emailToUserId.end()) {
      return false;
    }

    string id = IdGenerator::generateId();
    auto user = make_shared<User>(id, name, email, password, headline, summary,
                                  experience, education, skills, isRecruiter,
                                  weak_ptr<Company>());
    users[id] = user;
    emailToUserId[email] = id;
    return true;
  }

  shared_ptr<Session> login(const string &email, const string &password) {
    auto it = emailToUserId.find(email);
    if (it == emailToUserId.end()) {
      return nullptr;
    }

    auto userIt = users.find(it->second);
    if (userIt == users.end()) {
      return nullptr;
    }

    // In real system, check password hash
    // For demo, assume any password works
    string sessionId = IdGenerator::generateId();
    auto session = make_shared<Session>(sessionId, userIt->first);
    sessions[sessionId] = session;
    return session;
  }

  void logout(const string &sessionId) {
    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
      it->second->invalidate();
      sessions.erase(it);
    }
  }

  shared_ptr<User> getUser(const string &userId) {
    auto it = users.find(userId);
    if (it != users.end()) {
      return it->second;
    }
    return nullptr;
  }

  bool validateSession(const string &sessionId) {
    auto it = sessions.find(sessionId);
    return it != sessions.end() && it->second->isActive();
  }

  vector<shared_ptr<User>> getAllUsers() const {
    vector<shared_ptr<User>> allUsers;
    for (const auto &pair : users) {
      allUsers.push_back(pair.second);
    }
    return allUsers;
  }

  vector<shared_ptr<User>> searchUsersByName(const string &query) const {
    vector<shared_ptr<User>> results;
    for (const auto &pair : users) {
      auto user = pair.second;
      if (user->getName().find(query) != string::npos ||
          user->getHeadline().find(query) != string::npos) {
        results.push_back(user);
      }
    }
    return results;
  }
};

class UserService {
private:
  shared_ptr<AuthService> authService;

public:
  UserService(shared_ptr<AuthService> auth) : authService(auth) {}

  bool updateUserProfile(const string &userId, const string &headline,
                         const string &summary, const string &experience,
                         const string &education) {
    auto user = authService->getUser(userId);
    if (!user)
      return false;

    user->changeHeadline(headline);
    // In real system, update other fields
    return true;
  }

  bool addUserSkill(const string &userId, const string &skill) {
    auto user = authService->getUser(userId);
    if (!user)
      return false;

    user->addSkill(skill);
    return true;
  }

  bool removeUserSkill(const string &userId, const string &skill) {
    auto user = authService->getUser(userId);
    if (!user)
      return false;

    user->removeSkill(skill);
    return true;
  }

  shared_ptr<User> getUser(const string &userId) {
    return authService->getUser(userId);
  }
};

class CompanyService {
private:
  unordered_map<string, shared_ptr<Company>> companies;
  shared_ptr<AuthService> authService;

public:
  CompanyService(shared_ptr<AuthService> auth) : authService(auth) {}

  shared_ptr<Company> createCompany(const string &name,
                                    const string &description,
                                    const string &industry,
                                    const string &location,
                                    const string &recruiterId) {
    auto recruiter = authService->getUser(recruiterId);
    if (!recruiter || !recruiter->getIsRecruiter()) {
      return nullptr;
    }

    string id = IdGenerator::generateId();
    auto company =
        make_shared<Company>(id, name, description, industry, location);
    companies[id] = company;
    company->addRecruiter(recruiter);
    return company;
  }

  shared_ptr<Company> getCompany(const string &companyId) {
    auto it = companies.find(companyId);
    return it != companies.end() ? it->second : nullptr;
  }
};

class ConnectionService {
private:
  unordered_map<string, vector<string>> userConnections;
  unordered_map<string, shared_ptr<ConnectionRequest>> pendingRequests;
  unordered_map<string, shared_ptr<Connection>> connections;
  shared_ptr<AuthService> authService;

public:
  ConnectionService(shared_ptr<AuthService> auth) : authService(auth) {}

  bool sendConnectionRequest(const string &senderId, const string &receiverId) {
    if (senderId == receiverId)
      return false;

    // Check if already connected
    auto connIt = userConnections.find(senderId);
    if (connIt != userConnections.end()) {
      if (find(connIt->second.begin(), connIt->second.end(), receiverId) !=
          connIt->second.end()) {
        return false;
      }
    }

    // Check for pending request
    for (const auto &req : pendingRequests) {
      if ((req.second->getSenderId() == senderId &&
           req.second->getReceiverId() == receiverId) ||
          (req.second->getSenderId() == receiverId &&
           req.second->getReceiverId() == senderId)) {
        if (req.second->getStatus() == ConnectionRequestStatus::PENDING) {
          return false;
        }
      }
    }

    string id = IdGenerator::generateId();
    auto request = make_shared<ConnectionRequest>(id, senderId, receiverId);
    pendingRequests[id] = request;
    return true;
  }

  bool acceptConnectionRequest(const string &requestId) {
    auto it = pendingRequests.find(requestId);
    if (it == pendingRequests.end())
      return false;

    auto request = it->second;
    if (request->getStatus() != ConnectionRequestStatus::PENDING)
      return false;

    request->accept();

    // Create connection
    auto sender = authService->getUser(request->getSenderId());
    auto receiver = authService->getUser(request->getReceiverId());
    if (!sender || !receiver)
      return false;

    auto connection = make_shared<Connection>(sender, receiver);
    string connId = IdGenerator::generateId();
    connections[connId] = connection;

    userConnections[request->getSenderId()].push_back(request->getReceiverId());
    userConnections[request->getReceiverId()].push_back(request->getSenderId());

    pendingRequests.erase(it);
    return true;
  }

  bool rejectConnectionRequest(const string &requestId) {
    auto it = pendingRequests.find(requestId);
    if (it == pendingRequests.end())
      return false;

    it->second->reject();
    pendingRequests.erase(it);
    return true;
  }

  vector<string> getUserConnections(const string &userId) {
    auto it = userConnections.find(userId);
    if (it != userConnections.end()) {
      return it->second;
    }
    return {};
  }
};

class MessagingService {
private:
  unordered_map<string, shared_ptr<Conversation>> conversations;
  unordered_map<string, vector<string>> userConversations;
  shared_ptr<AuthService> authService;

public:
  MessagingService(shared_ptr<AuthService> auth) : authService(auth) {}

  string createConversation(const string &user1Id, const string &user2Id) {
    // Check if conversation already exists
    for (const auto &convId : userConversations[user1Id]) {
      auto conv = conversations[convId];
      auto participants = conv->getParticipants();
      if (participants.size() == 2) {
        if ((participants[0]->getId() == user1Id &&
             participants[1]->getId() == user2Id) ||
            (participants[0]->getId() == user2Id &&
             participants[1]->getId() == user1Id)) {
          return convId;
        }
      }
    }

    auto user1 = authService->getUser(user1Id);
    auto user2 = authService->getUser(user2Id);
    if (!user1 || !user2)
      return "";

    string id = IdGenerator::generateId();
    vector<shared_ptr<User>> participants = {user1, user2};
    auto conversation = make_shared<Conversation>(id, participants);
    conversations[id] = conversation;
    userConversations[user1Id].push_back(id);
    userConversations[user2Id].push_back(id);
    return id;
  }

  bool sendMessage(const string &conversationId, const string &senderId,
                   const string &content) {
    auto convIt = conversations.find(conversationId);
    if (convIt == conversations.end())
      return false;

    auto sender = authService->getUser(senderId);
    if (!sender)
      return false;

    string messageId = IdGenerator::generateId();
    auto message = make_shared<Message>(messageId, sender, content);
    convIt->second->sendMessage(message);
    return true;
  }

  vector<shared_ptr<Message>>
  getConversationMessages(const string &conversationId) {
    auto it = conversations.find(conversationId);
    if (it != conversations.end()) {
      return it->second->getMessages();
    }
    return {};
  }
};

class JobService {
private:
  unordered_map<string, shared_ptr<JobPosting>> jobPostings;
  unordered_map<string, shared_ptr<JobApplication>> applications;
  unordered_map<string, vector<string>> userApplications;
  shared_ptr<AuthService> authService;
  shared_ptr<CompanyService> companyService;

public:
  JobService(shared_ptr<AuthService> auth, shared_ptr<CompanyService> company)
      : authService(auth), companyService(company) {}

  shared_ptr<JobPosting> postJob(const string &companyId, const string &title,
                                 const string &description,
                                 const string &requirements,
                                 const string &location) {
    auto company = companyService->getCompany(companyId);
    if (!company)
      return nullptr;

    string id = IdGenerator::generateId();
    auto job = make_shared<JobPosting>(id, title, description, requirements,
                                       location, company);
    jobPostings[id] = job;
    company->postJob(job);
    return job;
  }

  bool applyToJob(const string &userId, const string &jobId) {
    auto user = authService->getUser(userId);
    auto job = getJob(jobId);
    if (!user || !job || job->getStatus() == JobPostingStatus::CLOSED)
      return false;

    // Check if already applied
    for (const auto &appId : userApplications[userId]) {
      auto app = applications[appId];
      if (app && app->getJob()->getId() == jobId) {
        return false;
      }
    }

    string id = IdGenerator::generateId();
    auto application = make_shared<JobApplication>(id, user, job);
    applications[id] = application;
    userApplications[userId].push_back(id);
    return true;
  }

  bool updateApplicationStatus(const string &applicationId,
                               JobApplicationStatus status) {
    auto it = applications.find(applicationId);
    if (it == applications.end())
      return false;

    it->second->updateStatus(status);
    return true;
  }

  shared_ptr<JobPosting> getJob(const string &jobId) {
    auto it = jobPostings.find(jobId);
    return it != jobPostings.end() ? it->second : nullptr;
  }

  vector<shared_ptr<JobApplication>> getUserApplications(const string &userId) {
    vector<shared_ptr<JobApplication>> result;
    auto it = userApplications.find(userId);
    if (it != userApplications.end()) {
      for (const auto &appId : it->second) {
        auto appIt = applications.find(appId);
        if (appIt != applications.end()) {
          result.push_back(appIt->second);
        }
      }
    }
    return result;
  }
};

class NotificationService {
private:
  unordered_map<string, shared_ptr<Notification>> notifications;
  unordered_map<string, vector<string>> userNotifications;
  shared_ptr<AuthService> authService;

public:
  NotificationService(shared_ptr<AuthService> auth) : authService(auth) {}

  void sendNotification(const string &userId, const string &message,
                        NotificationType type) {
    auto user = authService->getUser(userId);
    if (!user)
      return;

    string id = IdGenerator::generateId();
    auto notification = make_shared<Notification>(id, user, message, type);
    notifications[id] = notification;
    userNotifications[userId].push_back(id);
  }

  vector<shared_ptr<Notification>> getUserNotifications(const string &userId) {
    vector<shared_ptr<Notification>> result;
    auto it = userNotifications.find(userId);
    if (it != userNotifications.end()) {
      for (const auto &notifId : it->second) {
        auto notifIt = notifications.find(notifId);
        if (notifIt != notifications.end()) {
          result.push_back(notifIt->second);
        }
      }
    }
    return result;
  }

  bool markNotificationAsRead(const string &notificationId) {
    auto it = notifications.find(notificationId);
    if (it == notifications.end())
      return false;

    it->second->markAsRead();
    return true;
  }
};

class SearchService {
private:
  unordered_map<string, shared_ptr<User>> users;
  unordered_map<string, shared_ptr<JobPosting>> jobPostings;
  shared_ptr<AuthService> authService;

public:
  SearchService(shared_ptr<AuthService> auth) : authService(auth) {}

  vector<shared_ptr<User>> searchUsers(const string &query) {
    return authService->searchUsersByName(query);
  }

  void addJobPosting(shared_ptr<JobPosting> job) {
    jobPostings[job->getId()] = job;
  }

  vector<shared_ptr<JobPosting>> searchJobs(const string &query) {
    vector<shared_ptr<JobPosting>> results;
    for (const auto &job : jobPostings) {
      auto jobPtr = job.second;
      if (jobPtr->getTitle().find(query) != string::npos ||
          jobPtr->getStatus() == JobPostingStatus::OPEN) {
        results.push_back(jobPtr);
      }
    }
    return results;
  }
};

int main() {
  cout << "LinkedIN System" << endl;
  cout << "===============" << endl;

  // Create services
  auto authService = make_shared<AuthService>();
  auto userService = make_shared<UserService>(authService);
  auto companyService = make_shared<CompanyService>(authService);
  auto connectionService = make_shared<ConnectionService>(authService);
  auto messagingService = make_shared<MessagingService>(authService);
  auto jobService = make_shared<JobService>(authService, companyService);
  auto notificationService = make_shared<NotificationService>(authService);
  auto searchService = make_shared<SearchService>(authService);

  // Register users
  cout << "\n1. Registering users..." << endl;
  authService->registerUser(
      "John Doe", "john@example.com", "pass123", "Software Engineer at Google",
      "Experienced software developer", "5 years at Google",
      "B.S. Computer Science", {"C++", "Python", "Java"}, false);

  authService->registerUser("Jane Smith", "jane@example.com", "pass456",
                            "Senior Developer at Microsoft",
                            "Full-stack developer", "8 years at Microsoft",
                            "M.S. Computer Science",
                            {"JavaScript", "React", "Node.js"}, false);

  authService->registerUser(
      "HR Manager", "hr@techcorp.com", "pass789", "HR Manager at TechCorp",
      "Human Resources professional", "10 years in HR", "MBA HR",
      {"Recruiting", "Talent Management"}, true);

  // Login
  cout << "\n2. Logging in..." << endl;
  auto session1 = authService->login("john@example.com", "pass123");
  auto session2 = authService->login("jane@example.com", "pass456");
  auto session3 = authService->login("hr@techcorp.com", "pass789");

  if (session1 && session2 && session3) {
    cout << "All users logged in successfully!" << endl;
  }

  // Get user IDs
  string johnId = session1->getUserId();
  string janeId = session2->getUserId();
  string hrId = session3->getUserId();

  // Create company
  cout << "\n3. Creating company..." << endl;
  auto company =
      companyService->createCompany("TechCorp Inc.", "Leading tech company",
                                    "Technology", "San Francisco, CA", hrId);
  if (company) {
    cout << "Company created: " << company->getName() << endl;
  }

  // Post a job
  cout << "\n4. Posting a job..." << endl;
  auto job = jobService->postJob(company->getId(), "Senior Software Engineer",
                                 "Looking for experienced C++ developer",
                                 "5+ years C++, system design experience",
                                 "San Francisco, CA");
  if (job) {
    cout << "Job posted: " << job->getTitle() << endl;
    searchService->addJobPosting(job);
  }

  // Send connection request
  cout << "\n5. Sending connection request..." << endl;
  if (connectionService->sendConnectionRequest(johnId, janeId)) {
    cout << "Connection request sent from John to Jane" << endl;
  }

  // Accept connection request
  cout << "\n6. Accepting connection request..." << endl;
  // In real system, would need to get request ID
  // For demo, we'll use a simplified approach

  // Send message
  cout << "\n7. Sending message..." << endl;
  string convId = messagingService->createConversation(johnId, janeId);
  if (!convId.empty()) {
    if (messagingService->sendMessage(
            convId, johnId, "Hi Jane, would you like to collaborate?")) {
      cout << "Message sent successfully!" << endl;
    }
  }

  // Apply to job
  cout << "\n8. Applying to job..." << endl;
  if (jobService->applyToJob(johnId, job->getId())) {
    cout << "John applied to the job successfully!" << endl;
  }

  // Send notification
  cout << "\n9. Sending notification..." << endl;
  notificationService->sendNotification(johnId,
                                        "Your application has been received",
                                        NotificationType::JOB_APPLICATION);

  // Search for users
  cout << "\n10. Searching for users..." << endl;
  auto searchResults = searchService->searchUsers("John");
  for (const auto &user : searchResults) {
    cout << "Found user: " << user->getName() << " - " << user->getHeadline()
         << endl;
  }

  cout << "\n===============" << endl;
  cout << "Demo completed successfully!" << endl;

  return 0;
}
