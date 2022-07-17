#include "MiniAudioSource.h"
#include "MiniAudioManager/MiniAudioManager.h"

ma_result MiniAudioManager::result;
ma_engine MiniAudioManager::engine;
bool MiniAudioManager::isEngineInitialized;

//void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
//{
//    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
//    if (pDecoder == NULL) {
//        return;
//    }
//
//    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);
//
//    (void)pInput;
//}

void MiniAudioSource::LoadAudioFile(std::string file)
{
    audioFile = file;

    if(MiniAudioManager::isEngineInitialized)
        ma_sound_init_from_file(&MiniAudioManager::engine, audioFile.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, &internalSound);
    else
        setAudioLoadingOnUpdate = true;
}

void MiniAudioSource::Play()
{
    //ma_engine_play_sound(&MiniAudioManager::engine, &internalSound, NULL);

    if(!isPlaying)
    {
        if(MiniAudioManager::isEngineInitialized)
        {
            ma_result r = ma_sound_start(&internalSound);
            if(r != MA_SUCCESS)
            {
                std::cout << "[WARNING] Failed to start sound." << std::endl;
            }
            //else if(r == MA_SUCCESS)
                //std::cout << "Playing Audio!" << std::endl;
        }
        else
            std::cout << "[WARNING] Mini Audio Engine is not initialized!" << std::endl;

        isPlaying = true;
    }
}

void MiniAudioSource::Stop()
{   
    if(MiniAudioManager::isEngineInitialized)
    {
        ma_sound_seek_to_pcm_frame(&internalSound, 0);
        ma_sound_stop(&internalSound);
        isPlaying = false;
    }
}

void MiniAudioSource::OnStartup()
{
    RequireStaticScript(MiniAudioManager);
}

void MiniAudioSource::PlayOnStart() { playOnStart = true; }

void MiniAudioSource::OnUpdate()
{
    if(MiniAudioManager::isEngineInitialized)
    {
        if(setAudioLoadingOnUpdate)
        {
            LoadAudioFile(audioFile);
            setAudioLoadingOnUpdate = false;
        }

        if(playOnStart && !isPlaying)
        {
            //std::cout << "Playing On Start!" << std::endl;
            Play();
        }
    }
}

void MiniAudioSource::SetTime(float milliseconds)
{
    Stop();

    int frameResult = ma_engine_get_sample_rate(&MiniAudioManager::engine) * (milliseconds / 1000);

    ma_sound_seek_to_pcm_frame(&internalSound, frameResult);
    Play();
}