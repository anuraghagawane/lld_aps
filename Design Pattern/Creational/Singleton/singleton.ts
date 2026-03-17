class CacheManager {
  private cache = new Map<string, {value: string, expiry: number | null}> ();

  put(key: string, value:string, ttlSeconds: number = 0): void {
    const expiry = ttlSeconds > 0 ? Date.now() + ttlSeconds * 1000 : null;
    this.cache.set(key, {value, expiry});
  }

  get(key: string): string | null {
    const entry = this.cache.get(key);
    if(!entry) return null;
    if(entry.expiry !== null && Date.now() > entry.expiry) {
      this.cache.delete(key);
      return null;
    }

    return entry.value;
  }

  remove(key: string): void {
    this.cache.delete(key);
  }

  size(): number {
    const now = Date.now();
    for(const [key, entry] of this.cache) {
      if(entry.expiry !== null && now > entry.expiry) {
        this.cache.delete(key);
      }
    }
    return this.cache.size;
  }
}

// ES module singleton
export const cacheManager = new CacheManager();

// --- Main ---
const cache1 = cacheManager;
const cache2 = cacheManager;

console.log(`Same instance? ${cache1 === cache2}`); // true

// Component A caches data
cache1.put("user:42", "{name: 'Alice'}", 5); // 5-second TTL
cache1.put("config:theme", "dark");           // no expiry

// Component B reads from the same cache
console.log(`user:42 = ${cache2.get("user:42")}`);         // {name: 'Alice'}
console.log(`config:theme = ${cache2.get("config:theme")}`); // dark
console.log(`Cache size: ${cache2.size()}`);                 // 2
