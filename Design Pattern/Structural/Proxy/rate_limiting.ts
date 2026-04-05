interface ApiService {
    request(endpoint: string): string;
}

class RealApiService implements ApiService {
    request(endpoint: string): string {
        return "Response from " + endpoint;
    }
}

class RateLimitingProxy implements ApiService {
    // TODO: Add a field to store the real ApiService reference
    // TODO: Add an array to store timestamps of recent requests
    // TODO: Add a field for MAX_REQUESTS (3)
    // TODO: Add a field for TIME_WINDOW_MS (10000)
    private realService: ApiService;
    private timestamps: number[] = [];
    private static readonly MAX_REQUESTS = 3;
    private static readonly TIME_WINDOW_MS = 10_000;

    constructor(service: ApiService) {
        // TODO: Store the real service reference
        // TODO: Initialize the timestamps array
        this.realService = service;
    }

    request(endpoint: string): string {
        // TODO: Get the current time using Date.now()
        // TODO: Remove timestamps older than TIME_WINDOW_MS from the array
        // TODO: If the number of timestamps >= MAX_REQUESTS, return "Rate limit exceeded. Try again later."
        // TODO: Add the current timestamp to the array
        // TODO: Delegate to the real service's request() and return the result
        const now = Date.now();
        this.timestamps = this.timestamps.filter(t => now - t <= RateLimitingProxy.TIME_WINDOW_MS);
        if(this.timestamps.length >= RateLimitingProxy.MAX_REQUESTS) {
            return "Rate limit exceeded. Try again later.";
        }
        this.timestamps.push(now);
        return this.realService.request(endpoint);;
    }
}

const api: ApiService = new RateLimitingProxy(new RealApiService()); // Replace with RateLimitingProxy

console.log(api.request("/users"));
console.log(api.request("/orders"));
console.log(api.request("/products"));
console.log(api.request("/inventory")); // Should be rejected
