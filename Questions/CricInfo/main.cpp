#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

enum class PrimaryRole { BATTER, BOWLER };

class Player {
private:
  string id;
  string name;
  PrimaryRole role;

public:
  Player(const string &id, const string &name, PrimaryRole role)
      : id(id), name(name), role(role) {}

  // getters
  string getName() const { return name; }
  string getId() const { return id; }
  PrimaryRole getRole() const { return role; }
};

class Team {
private:
  string id;
  string name;
  string country;
  vector<shared_ptr<Player>> squad;

public:
  Team(const string &id, const string &name, const string &country,
       vector<shared_ptr<Player>> squad)
      : id(id), name(name), country(country), squad(squad) {}

  // getters
  string getName() const { return name; }
  string getId() const { return id; }
  string getCountry() const { return country; }

  // setters
  void insertPlayer(shared_ptr<Player> player) { squad.push_back(player); }
};

class Venue {
private:
  string id;
  string name;
  string city;
  string country;

public:
  Venue(const string &id, const string &name, const string &country,
        const string &city)
      : id(id), name(name), country(country), city(city) {}

  // getters
  string getName() const { return name; }
  string getId() const { return id; }
  string getCountry() const { return country; }
  string getCity() const { return city; }
};

enum class ExtraType { WIDE, NO_BALL, NONE };

enum class DismissalType { BOWLED, RUNOUT, STUMPED, HITWICKET, NONE };

class Ball {
private:
  string id;
  int overNumber;
  int ballNumber;

  shared_ptr<Player> batter;
  shared_ptr<Player> nonStriker;
  shared_ptr<Player> bowler;

  int runs;
  bool wicket;
  ExtraType extras;
  DismissalType dismissal;

public:
  // constructor
  Ball(const string &id, const int overNumber, const int ballNumber,
       shared_ptr<Player> batter, shared_ptr<Player> nonStriker,
       shared_ptr<Player> bowler, const int runs, const bool wicket,
       ExtraType extras, DismissalType dismissal)
      : id(id), overNumber(overNumber), ballNumber(ballNumber), batter(batter),
        nonStriker(nonStriker), bowler(bowler), runs(runs), wicket(wicket),
        extras(extras), dismissal(dismissal) {}

  // getters
  int getOver() const { return overNumber; }
  int getBall() const { return ballNumber; }
  shared_ptr<Player> getBatter() const { return batter; }
  shared_ptr<Player> getNonStriker() const { return nonStriker; }
  shared_ptr<Player> getBowler() const { return bowler; }
  int getRuns() const { return runs; }
  bool isWicket() const { return wicket; }
  ExtraType getExtras() const { return extras; }
  DismissalType getDismissalType() const { return dismissal; }

  // updaters
  void changeRun(const int runs) { this->runs = runs; }
  void changeWicket(const bool wicket) { this->wicket = wicket; }
  // can add more if needed
};

class Commentary {
private:
  string id;
  string text;
  optional<string> ballId;
  time_t timestamp;

public:
  Commentary(const string &id, const string &text, optional<string> ballId)
      : id(id), text(text), ballId(ballId) {
    timestamp = time(nullptr);
  }

  // getters
  string getCommentary() const { return text; }
  time_t getTime() const { return timestamp; }
};

class ScoreSummary {
private:
  int runs;
  int wickets;
  int legalBalls;
  shared_ptr<Player> striker; // no storeing individual score for simplicity but
                              // we can add one more entity for player score
  shared_ptr<Player> nonStriker;
  shared_ptr<Player> currentBowler;

public:
  ScoreSummary(shared_ptr<Player> striker, shared_ptr<Player> nonStriker,
               shared_ptr<Player> bowler)
      : striker(striker), nonStriker(nonStriker), currentBowler(bowler),
        runs(0), wickets(0), legalBalls(0) {}
  // getters
  int getRuns() const { return runs; }
  int getWickets() const { return wickets; }
  int getBalls() const { return legalBalls; }
  string getStriker() const { return striker->getName(); }
  string getNonStriker() const { return nonStriker->getName(); }
  string getBowler() const { return currentBowler->getName(); }

  // setters/updaters
  void updateRuns(const int runs) { this->runs = runs; }
  void updateWickets(const int wickets) { this->wickets = wickets; }
  void updateLegalBalls(const int balls) { this->legalBalls = balls; }
  void updateBatter(shared_ptr<Player> striker, shared_ptr<Player> nonStriker) {
    this->striker = striker;
    this->nonStriker = nonStriker;
  }

  void updateBaller(shared_ptr<Player> bowler) { this->currentBowler = bowler; }

  void update(shared_ptr<Ball> ball) {
    this->updateRuns(runs + ball->getRuns());
    if (ball->getExtras() == ExtraType::NONE) {
      updateLegalBalls(legalBalls + 1);
    }
    if (ball->isWicket()) {
      updateWickets(wickets + 1);
    }
  }
};

class Inning {
private:
  int inningNumber;
  shared_ptr<Team> battingTeam;
  shared_ptr<Team> bowlingTeam;

  vector<shared_ptr<Ball>> balls;

  vector<shared_ptr<Commentary>> commentary;

  shared_ptr<ScoreSummary> scoreSummary;

public:
  Inning(const int inningNumber, shared_ptr<Team> battingTeam,
         shared_ptr<Team> bowlingTeam, shared_ptr<ScoreSummary> scoreSummary)
      : inningNumber(inningNumber), battingTeam(battingTeam),
        bowlingTeam(bowlingTeam), scoreSummary(scoreSummary) {}

  // get
  int getInningNumber() const { return inningNumber; }

  shared_ptr<Team> getBattingTeam() const { return battingTeam; };

  shared_ptr<Team> getBowlingTeam() const { return bowlingTeam; };

  shared_ptr<ScoreSummary> getScoreSummary() const { return scoreSummary; };

  const vector<shared_ptr<Ball>> &getBalls() const { return balls; };

  const vector<shared_ptr<Commentary>> &getCommentaries() const {
    return commentary;
  };

  // setters
  void addBall(shared_ptr<Ball> ball) {
    balls.push_back(ball);
    scoreSummary->update(ball);
  }

  void addCommentary(shared_ptr<Commentary> commentary) {
    this->commentary.push_back(commentary);
  }

  void displayScore() {
    cout << endl;
    cout << "Inning: " << inningNumber << endl;
    cout << "Run: " << scoreSummary->getRuns() << endl;
    cout << "Overs: " << scoreSummary->getBalls() / 6 << "."
         << scoreSummary->getBalls() % 6 << endl;
  }
};

enum class MatchStatus {
  DRAW,
  TEAM_A_WON,
  TEAM_B_WON,
  ONGOING,
};

enum class MatchType {
  TEST,
  ODI,
  T20,
};

class Match {
private:
  string id;

  shared_ptr<Team> teamA;
  shared_ptr<Team> teamB;

  shared_ptr<Venue> venue;

  vector<shared_ptr<Player>> playingXI_A;
  vector<shared_ptr<Player>> playingXI_B;

  vector<shared_ptr<Inning>> innings;

  optional<string> tournamentId;

  MatchStatus status;
  MatchType type;

public:
  Match(const string &id, shared_ptr<Team> teamA, shared_ptr<Team> teamB,
        shared_ptr<Venue> venue, vector<shared_ptr<Player>> playingXI_A,
        vector<shared_ptr<Player>> playingXI_B, MatchType type,
        optional<string> tournamentId)
      : id(id), teamA(teamA), teamB(teamB), venue(venue),
        playingXI_A(playingXI_A), playingXI_B(playingXI_B),
        status(MatchStatus::ONGOING), type(type), tournamentId(tournamentId) {}

  string getId() const { return id; }
  pair<shared_ptr<Team>, shared_ptr<Team>> getTeams() const {
    return {teamA, teamB};
  }

  vector<shared_ptr<Inning>> getInnings() const { return innings; }

  shared_ptr<Inning> getCurrentInning() const { return innings.back(); }

  MatchStatus getMatchStatus() const { return status; }

  MatchType getMatchType() const { return type; }

  void addInning(shared_ptr<Inning> inning) { innings.push_back(inning); }
  void updateStatus(MatchStatus status) { this->status = status; }

  void displayScore() {
    for (auto inning : innings) {
      inning->displayScore();
    }
  }
};

class Tournament {
private:
  string id;
  string name;
  string season;
  vector<shared_ptr<Match>> matches;

public:
  Tournament(const string &id, const string &name, const string &season,
             vector<shared_ptr<Match>> matches)
      : id(id), name(name), season(season), matches(matches) {}
  string getId() const { return id; }
  string getName() const { return name; }
  string getSeason() const { return season; }
  vector<shared_ptr<Match>> getMatches() const { return matches; }

  void addMatch(shared_ptr<Match> match) { matches.push_back(match); }
};

class MatchRepository {
private:
  unordered_map<string, shared_ptr<Match>> matches;

  MatchRepository() = default;

public:
  MatchRepository(const MatchRepository &) = delete;
  MatchRepository &operator=(const MatchRepository &) = delete;

  static MatchRepository &getInstance() {
    static MatchRepository instance;
    return instance;
  }
  void addMatch(shared_ptr<Match> match) { matches[match->getId()] = match; }

  shared_ptr<Match> getMatch(const string &matchId) {
    if (!matches.count(matchId)) {
      throw runtime_error("Match not found!!!");
    }
    return matches[matchId];
  }

  vector<shared_ptr<Match>> getMatches() {
    vector<shared_ptr<Match>> res;
    for (auto [id, match] : matches) {
      res.push_back(match);
    }
    return res;
  }
};

class MatchService {
private:
public:
  // getScore(id)
  void displayScore(const string &matchId) {
    MatchRepository::getInstance().getMatch(matchId)->displayScore();
  }
};

class TournamentService {
private:
  unordered_map<string, shared_ptr<Tournament>> tournaments;

public:
  void addTournament(const string &id, const string &name, const string &season,
                     vector<shared_ptr<Match>> matches) {
    auto tournament = make_shared<Tournament>(id, name, season, matches);
    tournaments[id] = tournament;
  }

  void addMatch(const string &tourId, shared_ptr<Match> match) {
    if (!tournaments.count(tourId)) {
      throw runtime_error("Tournament does not exists");
    }
    tournaments[tourId]->addMatch(match);
  }

  Tournament &getTournament(const string &tourId) {
    if (!tournaments.count(tourId)) {
      throw runtime_error("Tournament does not exists");
    }

    return *tournaments[tourId];
  }

  vector<shared_ptr<Tournament>> getTournaments() {
    vector<shared_ptr<Tournament>> res;
    for (auto [id, t] : tournaments) {
      res.push_back(t);
    }

    return res;
  }
};

class LiveUpdateService {
private:
  unordered_map<string, vector<string>> subs;
  // matchid, vector<subids>
  //
public:
  void addSub(const string &matchId, const string &subId) {
    subs[matchId].push_back(subId);
  }

  void removeSub(const string &matchId, const string &subId) {
    auto thisSubs = subs[matchId];
    thisSubs.erase(remove(thisSubs.begin(), thisSubs.end(), subId),
                   thisSubs.end());
  }

  void publish(const string &matchId, shared_ptr<ScoreSummary> score) {
    cout << "Updated Score: " << endl;
    cout << "RUNS: " << score->getRuns() << endl;
    cout << "Overs: " << score->getBalls() / 6 << "." << score->getBalls() % 6
         << endl;

    // have a for loop here and push the score
  }
};

class ScoreUpdateService {
private:
  shared_ptr<LiveUpdateService> liveUpdateService;

public:
  ScoreUpdateService(shared_ptr<LiveUpdateService> liveUpdateService)
      : liveUpdateService(liveUpdateService) {}

  void updateBall(const string &matchId, shared_ptr<Ball> ball) {
    auto match = MatchRepository::getInstance().getMatch(matchId);
    auto currInning = match->getCurrentInning();
    currInning->addBall(ball);
    liveUpdateService->publish(matchId, currInning->getScoreSummary());
  }
};

class CommentaryService {
public:
  void addCommentary(const string &matchId, shared_ptr<Commentary> commentary) {
    auto match = MatchRepository::getInstance().getMatch(matchId);
    auto currInning = match->getCurrentInning();
    currInning->addCommentary(commentary);
  }
};

class SearchService {
private:
  shared_ptr<MatchService> matchService;
  shared_ptr<TournamentService> tournamentService;

public:
  SearchService(shared_ptr<MatchService> matchService,
                shared_ptr<TournamentService> tournamentService)
      : tournamentService(tournamentService), matchService(matchService) {}

  void searchByMatch(const string &teamA, const string &teamB) {
    auto matches = MatchRepository::getInstance().getMatches();

    for (auto match : matches) {
      auto [t1, t2] = match->getTeams();
      if ((t1->getName() == teamA && t2->getName() == teamB) ||
          (t1->getName() == teamB && t2->getName() == teamA)) {
        cout << "Found match: " << match->getId() << endl; // minimal for now
      }
    }
  }

  void searchByTournamentName(const string &name) {
    auto tournaments = tournamentService->getTournaments();
    for (auto t : tournaments) {
      if (t->getName() == name) {
        cout << "Found tournament: " << t->getId() << " " << t->getName()
             << endl;
      }
    }
  }
};

int main() {
  cout << "CricInfo" << endl;
  auto matchService = make_shared<MatchService>();
  auto tournamentService = make_shared<TournamentService>();
  auto liveUpdateService = make_shared<LiveUpdateService>();
  auto scoreUpdateService = make_shared<ScoreUpdateService>(liveUpdateService);
  auto commentaryService = make_shared<CommentaryService>();
  auto searchService =
      make_shared<SearchService>(matchService, tournamentService);
  auto pt1 = make_shared<Player>("pt1", "abc", PrimaryRole::BATTER);
  auto pt2 = make_shared<Player>("pt2", "abc", PrimaryRole::BOWLER);
  auto pt3 = make_shared<Player>("pt3", "abc", PrimaryRole::BATTER);
  auto pt4 = make_shared<Player>("pt4", "abc", PrimaryRole::BOWLER);
  vector<shared_ptr<Player>> t1 = {pt1, pt2};
  vector<shared_ptr<Player>> t2 = {pt3, pt4};
  auto teamA = make_shared<Team>("TA", "teamA", "In", t1);
  auto teamB = make_shared<Team>("TA", "teamA", "In", t2);
  auto venue = make_shared<Venue>("v1", "fasda", "fadsfa", "fadsfasd");
  auto tournament =
      make_shared<Tournament>("t1", "DPL", "s2", vector<shared_ptr<Match>>());
  auto m1 = make_shared<Match>("m1", teamA, teamB, venue, t1, t2,
                               MatchType::ODI, "t1");
  auto i1 = make_shared<Inning>(1, teamA, teamB,
                                make_shared<ScoreSummary>(pt1, pt2, pt4));
  m1->addInning(i1);
  tournament->addMatch(m1);
  MatchRepository::getInstance().addMatch(m1);

  auto b1 = make_shared<Ball>("b1", 0, 1, pt1, pt2, pt3, 2, false,
                              ExtraType::NONE, DismissalType::NONE);

  auto b2 = make_shared<Ball>("b2", 0, 2, pt1, pt2, pt3, 1, false,
                              ExtraType::NONE, DismissalType::NONE);

  scoreUpdateService->updateBall(m1->getId(), b1);
  scoreUpdateService->updateBall(m1->getId(), b2);

  matchService->displayScore(m1->getId());
  return 0;
}
