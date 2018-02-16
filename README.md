# C++ Urllib
### Encode and Decode URLs elegantly with C++

Urllib is a single file, header only C++ 11 library for building and decoding URLs. 

## Sample Usage

### Add the header
```C++
#include "Urllib.hpp"
```

### Encoding a URL

```C++
Urllib::Encode encoder; // Create the encoder
encoder.setBaseUrl("https://google.com"); // protocol://host.com/path
encoder.setParameter(std::make_pair<std::string, std::string> ("key1", "val1")); // If you want query parameters inserted as a pair
encoder.setParameter("key2", "val2"); // Keeping it simple
encoder.setParameter("key1", "new#val"); //Override the existing key

auto encoded = encoder.encode(); // Le magic

std::cout << "Encoded URL: " << encoded ; // https://google.com?key1=new%23val&key2=val2
```

### Decoding a URL

```C++
Urllib::Decode decoder; // Create the decoder
decoder.setUrl("https://google.com?key1=new%23val&key2=val2"); // Set the URL to decode

std::string decoded = decoder.decode(); // Let the magic happen

std::cout << "Decoded URL: " << decoded << std::endl; //  https://google.com?key1=new%23val&key2=val2
std::cout << "Decoded URL: " << decoder.getUrl() << std::endl; // Same as above
std::cout << "Decode Base URL: " << decoder.getBaseUrl() << std::endl; // https://google.com
auto parameters = decoder.getParameters(); // std::map<std::string, std::string>
for(auto parameter : parameters){
    std::cout << "Key:" << parameter.first << " Value:" << parameter.second;
    std::cout << std::endl;
}
```
