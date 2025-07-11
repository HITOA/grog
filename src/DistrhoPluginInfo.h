#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND   "Grog"
#ifdef GROG_IS_SYNTH
    #define DISTRHO_PLUGIN_NAME    "Grog Synth"
    #define DISTRHO_PLUGIN_URI     "https://github.com/HITOA/grog#synth"
    #define DISTRHO_PLUGIN_CLAP_ID "Grog.GrogSynth"
#else
    #define DISTRHO_PLUGIN_NAME    "Grog Effect"
    #define DISTRHO_PLUGIN_URI     "https://github.com/HITOA/grog#effect"
    #define DISTRHO_PLUGIN_CLAP_ID "Grog.GrogEffect"
#endif

#define DISTRHO_PLUGIN_BRAND_ID  GrgB

#ifdef GROG_IS_SYNTH
    #define DISTRHO_PLUGIN_UNIQUE_ID GrgS
#else
    #define DISTRHO_PLUGIN_UNIQUE_ID GrgE
#endif

#define DISTRHO_PLUGIN_HAS_UI               1
#define DISTRHO_PLUGIN_IS_RT_SAFE           1
#define DISTRHO_PLUGIN_NUM_INPUTS           2
#define DISTRHO_PLUGIN_NUM_OUTPUTS          2
#define DISTRHO_PLUGIN_WANT_PROGRAMS        0
#define DISTRHO_UI_FILE_BROWSER             0
#define DISTRHO_UI_USER_RESIZABLE           1
#define DISTRHO_UI_DEFAULT_WIDTH            512
#define DISTRHO_UI_DEFAULT_HEIGHT           512
#define DISTRHO_PLUGIN_WANT_TIMEPOS         1
#define DISTRHO_PLUGIN_WANT_MIDI_INPUT      1
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS   1

#ifdef GROG_IS_SYNTH
    #define DISTRHO_PLUGIN_IS_SYNTH 1
#endif

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED