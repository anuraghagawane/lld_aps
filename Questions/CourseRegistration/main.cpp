#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class Course {
private:
  string id;
  string courseCode;
  string name;
  string instructor;
  int maxCapacity;

public:
  Course(const string &id, const string &courseCode, const string &name,
         const string &instructor, int maxCapacity)
      : id(id), courseCode(courseCode), name(name), instructor(instructor),
        maxCapacity(maxCapacity) {}

  string getId() const { return id; }
  string getCourseCode() const { return courseCode; }
  string getName() const { return name; }
  string getInstructor() const { return instructor; }
  int getMaxCapacity() const { return maxCapacity; }
};

class Student {
private:
  string id;
  string name;

public:
  Student(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }
};

class Registration {
private:
  string registrationId;
  string studentId;
  string courseId;

public:
  Registration(const string &registrationId, const string &studentId,
               const string &courseId)
      : registrationId(registrationId), studentId(studentId),
        courseId(courseId) {}

  string getId() const { return registrationId; }
  string getStudentId() const { return studentId; }
  string getCourseId() const { return courseId; }
};

class CourseEnrollment {
private:
  string courseId;
  int currentEnrollment;
  mutex mtx;

  friend class RegistrationService;

public:
  CourseEnrollment(const string &courseId)
      : courseId(courseId), currentEnrollment(0) {}
};

class CourseService {
private:
  unordered_map<string, shared_ptr<Course>> coursesById;
  unordered_map<string, unique_ptr<CourseEnrollment>> enrollementsByCourseId;
  mutex mtx;

public:
  void addCourse(shared_ptr<Course> course) {
    lock_guard<mutex> lock(mtx);
    coursesById[course->getId()] = course;
    enrollementsByCourseId[course->getId()] =
        make_unique<CourseEnrollment>(course->getId());
  }

  shared_ptr<Course> getCourse(const string &courseId) {
    lock_guard<mutex> lock(mtx);
    if (!coursesById.count(courseId)) {
      throw runtime_error("course not present");
    }
    return coursesById[courseId];
  }

  CourseEnrollment *getCourseEnrollment(const string &courseId) {
    lock_guard<mutex> lock(mtx);
    if (!enrollementsByCourseId.count(courseId)) {
      throw runtime_error("course enrollment not present");
    }

    return enrollementsByCourseId[courseId].get();
  }

  vector<shared_ptr<Course>> searchByCourseCode(const string &courseCode) {
    lock_guard<mutex> lock(mtx);
    vector<shared_ptr<Course>> courses;
    for (auto [key, course] : coursesById) {
      if (course->getCourseCode() == courseCode) {

        courses.push_back(course);
      }
    }

    return courses;
  }

  vector<shared_ptr<Course>> searchByCourseName(const string &courseName) {
    lock_guard<mutex> lock(mtx);
    vector<shared_ptr<Course>> courses;
    for (auto [key, course] : coursesById) {
      if (course->getName() == courseName) {

        courses.push_back(course);
      }
    }

    return courses;
  }
};

class RegistrationService {
private:
  unordered_map<string, unique_ptr<Registration>> registrationsById;
  unordered_map<string, unordered_set<string>> registrationsByStudent;
  unordered_map<string, unique_ptr<Student>> students;
  CourseService &courseService;
  mutex mtx;

public:
  RegistrationService(CourseService &courseService)
      : courseService(courseService) {}

  void registerStudent(const string &studentId, const string &name) {
    lock_guard<mutex> lock(mtx);
    auto student = make_unique<Student>(studentId, name);
    registrationsByStudent[student->getId()] = {};
    students[student->getId()] = std::move(student);
  }

  bool registerCourse(const string &studentId, const string &courseId) {
    auto course = courseService.getCourse(courseId);
    auto courseEnrollment = courseService.getCourseEnrollment(courseId);

    lock_guard<mutex> courseLock(courseEnrollment->mtx);
    lock_guard<mutex> registrationLock(mtx);
    if (!students.count(studentId)) {
      throw runtime_error("student not present");
    }
    // check duplicates
    if (registrationsByStudent[studentId].count(courseId)) {
      cout << "Already registered" << endl;
      return false;
    }

    // check can enroll
    if (courseEnrollment->currentEnrollment >= course->getMaxCapacity()) {
      cout << "Capacity full" << endl;
      return false;
    }
    courseEnrollment->currentEnrollment++;
    // create registration
    // store registration
    const string registrationId = courseId + "_" + studentId;
    registrationsById[registrationId] =
        make_unique<Registration>(registrationId, studentId, courseId);
    registrationsByStudent[studentId].insert(courseId);

    cout << "Enrollement successfull: " << registrationId << endl;

    return true;
  }

  vector<shared_ptr<Course>>
  getStudentRegisteredCourses(const string &studentId) {
    unordered_set<string> courseIds;
    {
      lock_guard<mutex> lock(mtx);
      if (!registrationsByStudent.count(studentId)) {
        throw runtime_error("student not found");
      }
      courseIds = registrationsByStudent[studentId];
    }

    vector<shared_ptr<Course>> courses;
    for (auto courseId : courseIds) {
      auto course = courseService.getCourse(courseId);
      courses.push_back(course);
    }

    return courses;
  }
};

int main() {
  cout << "Course Registration" << endl;
  CourseService courseService;
  RegistrationService registrationService(courseService);

  auto course1 = make_shared<Course>("1", "C1", "DBMS", "ABS", 1);
  auto course2 = make_shared<Course>("2", "C2", "FLAT", "ABS", 3);
  courseService.addCourse(course1);
  courseService.addCourse(course2);

  for (auto course : courseService.searchByCourseCode("C2")) {
    cout << "course name: " << course->getName() << endl;
  }
  for (auto course : courseService.searchByCourseName("DBMS")) {
    cout << "course name: " << course->getName() << endl;
  }
  registrationService.registerStudent("S1", "AH");
  registrationService.registerStudent("S2", "MB");

  registrationService.registerCourse("S1", "1");
  registrationService.registerCourse("S1", "1");
  registrationService.registerCourse("S2", "1");

  auto courses = registrationService.getStudentRegisteredCourses("S1");
  for (auto course : courses) {
    cout << "name: " << course->getName() << endl;
  }
  return 0;
}
