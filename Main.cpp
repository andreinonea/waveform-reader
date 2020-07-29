#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>


struct WavFile
{
	char chunkId[5];			/* Contains the letters "RIFF" in ASCII form (0x52494646 big - endian form). */
	uint32_t chunkSize;			/* 36 + SubChunk2Size, or more precisely: 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
									This is the size of the rest of the chunk following this number.
									This is the size of the entire file in bytes minus 8 bytes
									for the two fields not included in this count: ChunkID and ChunkSize. */
	char format[5];				/* Contains the letters "WAVE" in ASCII form (0x57415645 big - endian form). */
	char formatChunkId[5];		/* Contains the letters "fmt " (0x666d7420 big-endian form). */
	uint32_t formatChunkSize;	/* 16 for PCM. This is the size of the rest of the Subchunk which follows this number. */
	uint16_t audioFormat;		/* PCM = 1 (i.e. Linear quantization). Values other than 1 indicate some form of compression. */
	uint16_t numChannels;		/* Mono = 1, Stereo = 2, etc. */
	uint32_t sampleRate;		/* 8000, 44100, etc. */
	uint32_t byteRate;			/* == SampleRate * NumChannels * BitsPerSample/8 */
	uint16_t blockAlign;		/* == NumChannels * BitsPerSample/8
									The number of bytes for one sample including all channels.
									I wonder what happens when this number isn't an integer? */
	uint16_t bitsPerSample;		/* 8 bits = 8, 16 bits = 16, etc. */

	// TODO:
	char infoChunkId[5];
	uint32_t infoChunkSize;


	char dataChunkId[5];
	uint32_t dataChunkSize;
	std::vector<char> data;
};

std::ostream& operator<<(std::ostream& out, WavFile& wav)
{
	out << "ChunkID:\t\t" << wav.chunkId << std::endl
		<< "ChunkSize:\t\t" << wav.chunkSize << std::endl
		<< "Format:\t\t\t" << wav.format << std::endl
		<< "FormatChunkID:\t\t" << wav.formatChunkId << std::endl
		<< "FormatChunkSize:\t" << wav.formatChunkSize << std::endl
		<< "AudioFormat:\t\t" << wav.audioFormat << std::endl
		<< "NumChannels:\t\t" << wav.numChannels << std::endl
		<< "SampleRate:\t\t" << wav.sampleRate << std::endl
		<< "ByteRate:\t\t" << wav.byteRate << std::endl
		<< "BlockAlign:\t\t" << wav.blockAlign << std::endl
		<< "BitsPerSample:\t\t" << wav.bitsPerSample << std::endl
		<< "DataChunkID:\t\t" << wav.dataChunkId << std::endl
		<< "DataChunkSize:\t\t" << wav.dataChunkSize;

	return out;
}

void ReadCharField(std::ifstream& in, char field[5])
{
	field[4] = '\0';
	in.read(field, sizeof(field) - 1);
}

void ReadIntField(std::ifstream& in, uint16_t field)
{
	in.read(reinterpret_cast<char*>(&field), sizeof(field));
}

void ReadField(std::ifstream& in, uint32_t field)
{
	in.read(reinterpret_cast<char*>(&field), sizeof(field));
}

std::ifstream& operator>>(std::ifstream& in, WavFile& wav)
{
	ReadCharField(in, wav.chunkId);
	ReadIntField(in, wav.chunkSize);
	ReadCharField(in, wav.format);
	ReadCharField(in, wav.formatChunkId);
	ReadIntField(in, wav.formatChunkSize);
	ReadIntField(in, wav.audioFormat);
	ReadIntField(in, wav.numChannels);
	ReadIntField(in, wav.sampleRate);
	ReadIntField(in, wav.byteRate);
	ReadIntField(in, wav.blockAlign);
	ReadIntField(in, wav.bitsPerSample);
	ReadCharField(in, wav.dataChunkId);
	ReadIntField(in, wav.dataChunkSize);


	/*in.read(wav.chunkId, sizeof(wav.chunkId) - 1);
	wav.chunkId[4] = '\0';
	in.read(reinterpret_cast<char*>(&wav.chunkSize), sizeof(wav.chunkSize));
	in.read(wav.format, sizeof(wav.format) - 1);
	wav.format[4] = '\0';
	in.read(wav.formatChunkId, sizeof(wav.formatChunkId) - 1);
	wav.formatChunkId[4] = '\0';
	in.read(reinterpret_cast<char*>(&wav.formatChunkSize), sizeof(wav.formatChunkSize));
	in.read(reinterpret_cast<char*>(&wav.audioFormat), sizeof(wav.audioFormat));
	in.read(reinterpret_cast<char*>(&wav.numChannels), sizeof(wav.numChannels));
	in.read(reinterpret_cast<char*>(&wav.sampleRate), sizeof(wav.sampleRate));
	in.read(reinterpret_cast<char*>(&wav.byteRate), sizeof(wav.byteRate));
	in.read(reinterpret_cast<char*>(&wav.blockAlign), sizeof(wav.blockAlign));
	in.read(reinterpret_cast<char*>(&wav.bitsPerSample), sizeof(wav.bitsPerSample));
	in.read(wav.dataChunkId, sizeof(wav.dataChunkId) - 1);
	wav.dataChunkId[4] = '\0';
	in.read(reinterpret_cast<char*>(&wav.dataChunkSize), sizeof(wav.dataChunkSize));
	in.read(wav.debug, sizeof(wav.debug) - 1);
	wav.debug[4] = '\0';*/
	
	return in;
}

bool ReadWav(WavFile& wav, const char* filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	
	if (!file)
	{
		std::cerr << "Cannot open wav file: " << filename << std::endl;
		return false;
	}

	file >> wav;
	file.close();

	return true;
}


int main()
{
	WavFile wav = { 0 };

	if (! ReadWav(wav, "../blast.wav"))
		std::exit(1);

	std::cout << wav << std::endl;


	//for (std::vector<char>::iterator it = wav.data.begin(); it != wav.data.end(); it++)
		//std::cout << *it << std::endl;

	//std::cin.get();
	return 0;
}