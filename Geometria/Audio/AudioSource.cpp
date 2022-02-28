#include "AudioSource.h"
#include "AudioManager/AudioManager.h"

ma_result AudioManager::result;
ma_decoder AudioManager::decoder;
ma_device_config AudioManager::deviceConfig;
ma_device AudioManager::device;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

    (void)pInput;
}

void AudioSource::Play()
{
    auto isRunning = ma_device_start(&AudioManager::device);
    if (isRunning != MA_SUCCESS)
    {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&AudioManager::device);
        ma_decoder_uninit(&AudioManager::decoder);
    }
    else if(isRunning == MA_SUCCESS)
    {
        isPlaying = true;
    }
}

void AudioSource::OnStart()
{
    std::cout << "Hello Audio!" << std::endl;

    AudioManager::result = ma_decoder_init_file(audioFile.c_str(), NULL, &AudioManager::decoder);
    if (AudioManager::result != MA_SUCCESS)
    {
        printf("Could not load file: %s\n", audioFile.c_str());
    }

    AudioManager::deviceConfig = ma_device_config_init(ma_device_type_playback);
    AudioManager::deviceConfig.playback.format = AudioManager::decoder.outputFormat;
    AudioManager::deviceConfig.playback.channels = AudioManager::decoder.outputChannels;
    AudioManager::deviceConfig.sampleRate = AudioManager::decoder.outputSampleRate;
    AudioManager::deviceConfig.dataCallback = data_callback;
    AudioManager::deviceConfig.pUserData = &AudioManager::decoder;

    if (ma_device_init(NULL, &AudioManager::deviceConfig, &AudioManager::device) != MA_SUCCESS)
    {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&AudioManager::decoder);
    }
}
