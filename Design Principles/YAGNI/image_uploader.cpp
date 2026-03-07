#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>

// Simple File class
class File {
public:
    std::string name;
    std::string content;

    File() = default;
    File(const std::string& name, const std::string& content)
        : name(name), content(content) {}
};

// Interface for handling different media types
class IMediaHandler {
public:
    virtual ~IMediaHandler() = default;
    virtual bool canHandle(const std::string& fileType) = 0;
    virtual File process(const File& file) = 0;
};

// Interface for storage providers
class IStorageProvider {
public:
    virtual ~IStorageProvider() = default;
    virtual void store(const File& file, const std::string& path) = 0;
    virtual File retrieve(const std::string& path) = 0;
    virtual void remove(const std::string& path) = 0;
};

// Factory for creating media handlers
class MediaHandlerFactory {
private:
    std::unordered_map<std::string, IMediaHandler*> handlers;

public:
    void registerHandler(const std::string& type, IMediaHandler* handler) {
        handlers[type] = handler;
    }

    IMediaHandler* getHandler(const std::string& fileType) {
        auto it = handlers.find(fileType);
        if (it == handlers.end()) {
            throw std::runtime_error("No handler for type: " + fileType);
        }
        return it->second;
    }
};

// Local storage provider (simple implementation)
class LocalStorageProvider : public IStorageProvider {
private:
    std::unordered_map<std::string, File> storage;

public:
    void store(const File& file, const std::string& path) override {
        storage[path] = file;
        std::cout << "Stored file '" << file.name << "' at path: " << path << "\n";
    }

    File retrieve(const std::string& path) override {
        if (storage.find(path) == storage.end()) {
            throw std::runtime_error("File not found: " + path);
        }
        return storage[path];
    }

    void remove(const std::string& path) override {
        storage.erase(path);
        std::cout << "Removed file at path: " << path << "\n";
    }
};

// Image handler (resizes image)
class ImageMediaHandler : public IMediaHandler {
public:
    bool canHandle(const std::string& fileType) override {
        return fileType == "image";
    }

    File process(const File& file) override {
        return resize(file, 300, 300);
    }

private:
    File resize(const File& file, int width, int height) {
        std::cout << "Resizing image '" << file.name 
                  << "' to " << width << "x" << height << "\n";
        // For simplicity, we just append "_resized" to content
        return File(file.name, file.content + "_resized");
    }
};

// Media processing engine
class MediaProcessingEngine {
private:
    MediaHandlerFactory* handlerFactory;
    IStorageProvider* storageProvider;

public:
    MediaProcessingEngine(MediaHandlerFactory* handlerFactory,
                          IStorageProvider* storageProvider)
        : handlerFactory(handlerFactory),
          storageProvider(storageProvider) {}

    void upload(const File& file, const std::string& fileType,
                const std::string& path) {
        IMediaHandler* handler = handlerFactory->getHandler(fileType);
        File processed = handler->process(file);
        storageProvider->store(processed, path);
    }
};

// ImageResizer class
class ImageResizer {
public:
    File resize(const File& file, int width, int height) {
        std::cout << "Resizing image '" << file.name 
                  << "' to " << width << "x" << height << "\n";
        // For simplicity, just append "_resized" to content
        return File(file.name, file.content + "_resized");
    }
};

// LocalStorage class
class LocalStorage {
private:
    std::unordered_map<std::string, File> storage;

public:
    void save(const File& file) {
        storage[file.name] = file;
        std::cout << "Saved file '" << file.name << "' locally.\n";
    }

    File get(const std::string& name) {
        if (storage.find(name) == storage.end()) {
            throw std::runtime_error("File not found: " + name);
        }
        return storage[name];
    }

    void remove(const std::string& name) {
        storage.erase(name);
        std::cout << "Removed file '" << name << "' from storage.\n";
    }
};

// ImageUploader class
class ImageUploader {
private:
    ImageResizer* resizer;
    LocalStorage* storage;

public:
    ImageUploader(ImageResizer* resizer, LocalStorage* storage)
        : resizer(resizer), storage(storage) {}

    void upload(const File& imageFile) {
        File resized = resizer->resize(imageFile, 300, 300);
        storage->save(resized);
    }
};

// Demo
int main() {
    // Setup
    // MediaHandlerFactory factory;
    // ImageMediaHandler imageHandler;
    // factory.registerHandler("image", &imageHandler);
    //
    // LocalStorageProvider localStorage;
    // MediaProcessingEngine engine(&factory, &localStorage);
    //
    // // Create a file
    // File myImage("photo.jpg", "raw_image_data");
    //
    // // Upload and process
    // engine.upload(myImage, "image", "/images/photo.jpg");
    //
    // // Retrieve it
    // File stored = localStorage.retrieve("/images/photo.jpg");
    // std::cout << "Retrieved file content: " << stored.content << "\n";

    ImageResizer resizer;
    LocalStorage storage;
    ImageUploader uploader(&resizer, &storage);

    File myImage("photo.jpg", "raw_image_data");

    // Upload image
    uploader.upload(myImage);

    // Retrieve and print content
    File stored = storage.get("photo.jpg");
    std::cout << "Retrieved file content: " << stored.content << "\n";

    return 0;
}
