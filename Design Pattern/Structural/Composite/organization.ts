interface OrgComponent {
  getSalary(): number;
  getHeadcount(): number;
  printHierarchy(indent: string): void;
}

class Employee implements OrgComponent {
  private readonly name: string;
  private readonly title: string;
  private readonly salary: number;

  constructor(name: string, title: string, salary: number) {
    this.name = name;
    this.title = title;
    this.salary = salary;
  }

  getSalary(): number {
    return this.salary;
  }

  getHeadcount(): number {
    return 1;
  }

  printHierarchy(indent: string): void {
    console.log(`${indent}- ${this.name} (${this.title}, $${this.salary})`);
  }
}

class Manager implements OrgComponent {
  private readonly name: string;
  private readonly title: string;
  private readonly salary: number;
  private readonly members: OrgComponent[] = [];

  constructor(name: string, title: string, salary: number) {
    this.name = name;
    this.title = title;
    this.salary = salary;
  }

  addMember(member: OrgComponent): void {
    this.members.push(member);
  }

  removeMember(member: OrgComponent): void {
    const index = this.members.indexOf(member);
    if (index !== -1) this.members.splice(index, 1);
  }

  getSalary(): number {
    let total = this.salary;
    for (const member of this.members) total += member.getSalary();
    return total;
  }

  getHeadcount(): number {
    let count = 1;
    for (const member of this.members) count += member.getHeadcount();
    return count;
  }

  printHierarchy(indent: string): void {
    console.log(`${indent}+ ${this.name} (${this.title}, $${this.salary})`);
    for (const member of this.members) member.printHierarchy(indent + "  ");
  }
}

const dev1 = new Employee("Alice", "Senior Engineer", 120000);
const dev2 = new Employee("Bob", "Engineer", 95000);
const dev3 = new Employee("Charlie", "Engineer", 90000);
const designer = new Employee("Diana", "Designer", 100000);

const techLead = new Manager("Eve", "Tech Lead", 140000);
techLead.addMember(dev1);
techLead.addMember(dev2);

const vpEng = new Manager("Frank", "VP Engineering", 200000);
vpEng.addMember(techLead);
vpEng.addMember(dev3);

const vpProduct = new Manager("Grace", "VP Product", 190000);
vpProduct.addMember(designer);

const ceo = new Manager("Hank", "CEO", 300000);
ceo.addMember(vpEng);
ceo.addMember(vpProduct);

console.log("---- Organization Chart ----");
ceo.printHierarchy("");

console.log("\nTotal Payroll: $" + ceo.getSalary());
console.log("Total Headcount: " + ceo.getHeadcount());
console.log("\nEngineering Payroll: $" + vpEng.getSalary());
console.log("Engineering Headcount: " + vpEng.getHeadcount());
