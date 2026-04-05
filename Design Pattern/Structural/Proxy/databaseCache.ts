interface DatabaseService {
  query(sql: string): string;
}

class RealDatabaseService implements DatabaseService {
  query(sql: string): string {
    console.log("RealDatabase: Executing query: " + sql);
    const start = Date.now();
    while(Date.now() - start < 2000) {}
    return "Result for [" + sql + "]";
  }
}

class CachingDatabaseProxy implements DatabaseService {
  private realService = new RealDatabaseService();
  private cache: Map<string, string> = new Map();

  query(sql: string): string {
    const cached = this.cache.get(sql);
    if(cached !== undefined) {
      console.log("CachingProxy: Cache HIT for: " + sql);
      return cached;
    }

    console.log("CachingProxy: Cache MISS for: ", sql);
    const result = this.realService.query(sql);
    this.cache.set(sql, result);
    return result;
  }

  clearCache(): void {
    console.log("CachingProxy: Cache cleared.")
    this.cache.clear();
  }
}

const db = new CachingDatabaseProxy();

console.log("--- First query (cache miss) ---");
console.log(db.query("SELECT * FROM users"));

console.log("\n--- Same query again (cache hit) ---");
console.log(db.query("SELECT * FROM users"));

console.log("\n--- Different query (cache miss) ---");
console.log(db.query("SELECT * FROM orders WHERE status = 'pending'"));

console.log("\n--- Clear cache and retry ---");
db.clearCache();
console.log(db.query("SELECT * FROM users"));
