class Notification {
  public read: boolean = false;
  public readonly message: string;
  public readonly type: string;

  constructor(message: string, type: string) {
    this.message = message;
    this.type = type;
  }

  markRead(): void {
    this.read = true;
  }

  toString(): string {
    const status = this.read ? "read" : "unread";
    return `[${this.type}] ${this.message} (${status})`;
  }
}

interface NotificationIterator {
  hasNext(): boolean;
  next(): Notification;
}

class NotificationCenter {
  private notifications: Notification[] = [];

  add(notification: Notification): void {
    this.notifications.push(notification);
  }

  getAt(index: number): Notification {
    return this.notifications[index];
  }

  getSize(): number {
    return this.notifications.length;
  }

  createIterator(): NotificationIterator {
    return new AllNotificationsIterator(this);
  }

  createFilteredIterator(type: string): NotificationIterator {
    return new FilteredIterator(this, type);
  }

  createUnreadIterator(): NotificationIterator {
    return new UnreadIterator(this);
  }
}

class AllNotificationsIterator implements NotificationIterator {
  private index = 0;
  private center: NotificationCenter;

  constructor(center: NotificationCenter) {
    this.center = center;
  }

  hasNext(): boolean {
    return this.index < this.center.getSize();
  }

  next(): Notification {
    return this.center.getAt(this.index++);
  }
}

class FilteredIterator implements NotificationIterator {
  private index = 0;
  private center: NotificationCenter;
  private type: string;

  constructor(center: NotificationCenter, type: string) {
    this.center = center;
    this.type = type;
    this.advanceToNext();
  }

  private advanceToNext(): void {
    while (
      this.index < this.center.getSize() &&
      this.center.getAt(this.index).type !== this.type
    ) {
      this.index++;
    }
  }

  hasNext(): boolean {
    return this.index < this.center.getSize();
  }

  next(): Notification {
    const n = this.center.getAt(this.index);
    this.index++;
    this.advanceToNext();
    return n;
  }
}

class UnreadIterator implements NotificationIterator {
  private index = 0;
  private center: NotificationCenter;

  constructor(center: NotificationCenter) {
    this.center = center;
    this.advanceToNext();
  }

  private advanceToNext(): void {
    while (
      this.index < this.center.getSize() &&
      this.center.getAt(this.index).read
    ) {
      this.index++;
    }
  }

  hasNext(): boolean {
    return this.index < this.center.getSize();
  }

  next(): Notification {
    const n = this.center.getAt(this.index);
    this.index++;
    this.advanceToNext();
    return n;
  }
}

const center = new NotificationCenter();
center.add(new Notification("Your order shipped", "EMAIL"));
center.add(new Notification("Flash sale today!", "PUSH"));
center.add(new Notification("Verify your number", "SMS"));
center.add(new Notification("Invoice ready", "EMAIL"));
center.add(new Notification("New login detected", "PUSH"));

center.getAt(0).markRead();
center.getAt(2).markRead();

console.log("--- All Notifications ---");
const all = center.createIterator();
while (all.hasNext()) {
  console.log("  " + all.next().toString());
}

console.log("\n--- Email Only ---");
const emails = center.createFilteredIterator("EMAIL");
while (emails.hasNext()) {
  console.log("  " + emails.next().toString());
}

console.log("\n--- Unread Only ---");
const unread = center.createUnreadIterator();
while (unread.hasNext()) {
  console.log("  " + unread.next().toString());
}
