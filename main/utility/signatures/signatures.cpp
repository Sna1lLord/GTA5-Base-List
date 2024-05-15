#include "signatures.hpp"
#include "../utility/logger/logger.hpp"

uint8_t ToHexadecimal(char character) {
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    else if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    else if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    }

    return 0xFF;
}

Scanner::Scanner(std::string signature) {
    signature.erase(std::remove(signature.begin(), signature.end(), ' '), signature.end());

    std::vector<uint8_t> byteVector;
    byteVector.reserve(signature.size());

    for (size_t i = 0, j = 0; i < signature.size(); i+=2, j++) {
        size++;

        if (signature[i] == '?') {
            byteVector.emplace_back('?');
            i--;
        }
        else {
            byteVector.emplace_back((ToHexadecimal(signature[i]) << 4 | ToHexadecimal(signature[i + 1])));
        }
    }

    signatureBytes = byteVector;
}

Result::Result(uintptr_t result) {
    address = result;
}

Result Result::Add(uintptr_t value) {
    return Result(address + value);
}

Result Result::Sub(uintptr_t value) {
    return Result(address - value);
}

Result Result::Rip(uintptr_t add) {
    uintptr_t result = address + add;
    return Result(result + (*(int32_t*)result + 0x4));
}

template <typename T>
T Result::Cast() {
    return (T)(address);
}

Scan::Scan(std::string signature, Module data) {
    Scanner scanner = Scanner(signature);

    uint8_t* bytes = (uint8_t*)(data.base);
    size_t size = data.size;
    uintptr_t base = data.base;

    size_t maxShift = scanner.size;
    size_t maxIndex = scanner.size - 1;
    size_t wildCardIndex = 0;
    for (size_t i = 0; i < maxIndex; i++) {
        if (scanner.signatureBytes[i] == '?') {
            maxShift = maxIndex - i;
            wildCardIndex = i;
        }
    }

    size_t shiftTable[UINT8_MAX + 1];
    for (size_t i = 0; i <= UINT8_MAX; i++) {
        shiftTable[i] = maxShift;
    }

    for (size_t i = wildCardIndex + 1; i < maxIndex - 1; i++) {
        shiftTable[scanner.signatureBytes[i]] = maxIndex - i;
    }

    for (size_t currentIndex = 0; currentIndex < size - scanner.size;) {
        for (size_t sigIndex = maxIndex; sigIndex >= 0; sigIndex--) {
            if (bytes[currentIndex + sigIndex] != scanner.signatureBytes[sigIndex] && scanner.signatureBytes[sigIndex] != '?') {
                currentIndex += shiftTable[bytes[currentIndex + maxIndex]];

                break;
            }
            else if (sigIndex == 0) {
                result = (uintptr_t)(currentIndex + base);
                return;
            }
        }
    }

    result = 0x0;
}

Result Scan::Get() {
    return Result(result);
}

static Signatures* signaturesInstance = nullptr;

Signatures* Signatures::Instance() {
	if (!signaturesInstance)
		signaturesInstance = new Signatures();

	return signaturesInstance;
}

void Signatures::Initialize() {
    auto timerStart = std::chrono::system_clock::now();

    definitions.swapchain = Scan("48 8B 0D ? ? ? ? 48 8B 01 44 8D 43").Get().Rip().Cast<IDXGISwapChain**>();

    definitions.frameCount = Scan("41 FF CF FF C0 83 F8 08 41 0F 44 C5 48 63 D8").Get().Sub(0x6).Rip(0x2).Cast<uint32_t*>();
    definitions.scriptVm = Scan("E8 ? ? ? ? 48 85 FF 48 89 1D").Get().Rip(0x1).Cast<uintptr_t>();

    definitions.nativeRegistrationTable = Scan("48 85 C0 75 0A 48 8D 05").Get().Sub(0x10).Rip().Cast<decltype(definitions.nativeRegistrationTable)>();
    definitions.getNativeHandler = Scan("48 85 C0 75 0A 48 8D 05").Get().Sub(0x5).Rip(0x1).Cast<decltype(definitions.getNativeHandler)>();
    definitions.nativeReturn = Scan("FF E3").Get().Add(0).Cast<PVOID>();
    definitions.fixVectors = Scan("83 79 18 00 48 8B D1 74 4A FF 4A").Get().Cast<decltype(definitions.fixVectors)>();

    definitions.transactionQueue = Scan("89 4C 24 24 48 8B 0D").Get().Add(0x4).Rip().Cast<__int64*>();
    definitions.constructBasket = Scan("45 8B CC 44 8B C5 BA").Get().Sub(0x72).Cast<decltype(definitions.constructBasket)>();
    definitions.addItemToBasket = Scan("57 48 83 EC 20 48 8B F2 48 8D 54 24").Get().Sub(0xA).Cast<decltype(definitions.addItemToBasket)>();
    definitions.processTransaction = Scan("83 7B 40 01 48 8B 9C 24 C8 00").Get().Add(0x18).Rip(0x1).Cast<decltype(definitions.processTransaction)>();

    definitions.gameState = Scan("83 3D ? ? ? ? ? 75 17 8B 43 20 25").Get().Rip(0x2).Add(1).Cast<int*>();

    definitions.pedFactory = Scan("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52").Get().Rip().Cast<CPedFactory**>();

    definitions.onPlayerJoin = Scan("8B 55 38 4C 8D 45 D8 48 8D 0D").Get().Add(0x2B).Cast<uintptr_t>();
    definitions.onPlayerLeave = Scan("8B 41 28 85 C0 74 0B FF C8 89 41 28 48 83 64 C1").Get().Sub(0xFC).Cast<uintptr_t>();

    auto timerEnd = std::chrono::system_clock::now();

    auto time = std::chrono::duration<float>(timerEnd - timerStart).count();
    Logger::Instance()->Log("Found Signatures In %fs \n", time);
}

void Signatures::Uninitialize() {
    signaturesInstance = nullptr;
    delete this;
}