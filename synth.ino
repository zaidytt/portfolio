#include <MIDI.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_midi.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include <AutoMap.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/cos2048_int8.h>
#include <StateVariable.h>
#include <mozzi_rand.h>

Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);
StateVariable <NOTCH> svf;

#define LFOratePot A1
AutoMap LFOratePotMap(0, 1023, 40, 750);

#define LFOresPot A0
AutoMap LFOresPotMap(0, 1023, 40, 180);

LowPassFilter lpf;

MIDI_CREATE_DEFAULT_INSTANCE();

#define WAVE_SWITCH 2
#define LFO_ON_OFF_SWITCH 3

#define atkPot A5
AutoMap atkPotMap(0, 1023, 0, 3000);
#define dkyPot A4
AutoMap dkyPotMap(0, 1023, 0, 3000);

#define cutoffPot A2
AutoMap cutOffPotMap(0, 1023, 20, 250);

#define resPot A3
AutoMap resFreqPotMap(0, 1023, 40, 210);

#define CONTROL_RATE 128

Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscil1;
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscil2;

ADSR<CONTROL_RATE, AUDIO_RATE> envelope;

#define LED 13

void HandleNoteOn(byte channel, byte note, byte velocity) {
  oscil1.setFreq(mtof(float(note)));
  envelope.noteOn();
  digitalWrite(LED,HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) {
  envelope.noteOff();
  digitalWrite(LED,LOW);
}

void setup() {
  pinMode(LED, OUTPUT);
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  envelope.setADLevels(255, 64);
  oscil1.setFreq(440);
  kFilterMod.setFreq(1.3f);
  pinMode(WAVE_SWITCH, INPUT_PULLUP);
  pinMode(LFO_ON_OFF_SWITCH, INPUT_PULLUP);
  startMozzi(CONTROL_RATE);
}

void updateControl(){
  MIDI.read();

  envelope.setTimes(
    atkPotMap(mozziAnalogRead(atkPot)),
    60000,
    60000,
    dkyPotMap(mozziAnalogRead(dkyPot))
  );
  envelope.update();

  lpf.setResonance(resFreqPotMap(mozziAnalogRead(resPot)));
  lpf.setCutoffFreq(cutOffPotMap(mozziAnalogRead(cutoffPot)));

  int LFOrateVal = LFOratePotMap(mozziAnalogRead(LFOratePot));
  int LFOresVal = LFOresPotMap(mozziAnalogRead(LFOresPot));

  if (digitalRead(LFO_ON_OFF_SWITCH) == HIGH) {
    if (rand(CONTROL_RATE / 2) == 0) {
      kFilterMod.setFreq(LFOrateVal / 64);
    }
    svf.setCentreFreq(2200 + kFilterMod.next() * 12);
    svf.setResonance(LFOresVal);
  } else {
    svf.setCentreFreq(0);
    svf.setResonance(200);
  }

  oscil1.setTable(digitalRead(WAVE_SWITCH) == HIGH ? SAW2048_DATA : SQUARE_NO_ALIAS_2048_DATA);
}

int updateAudio() {
  int asig = (envelope.next() * oscil1.next()) >> 10;
  int asigSVF = svf.next(asig);
  int asigLPF = lpf.next(asigSVF);
  return asigLPF;
}

void loop() {
  audioHook();
}
