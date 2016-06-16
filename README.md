MelodySpeaker
=============

A Library for playing mono-tone melodies over a piezo speaker using the `tone()` function.
You can set playback tempo, and notes to play following the note encoding defined below.
The melody can be played either in blocking and non-blocking way.
In blocking mode the first call of `processMelody` will play the complete melody and return at the end.
In non-blocking mode the `MelodySpeaker` class tries to execute the melody following the system clock.
In this case the `processMelody` method must be called as often as possible.
If the execution of `processMelody` is delayed by other CPU-load, the playback of next note of the melody will be delayed as well.

## Melody encoding

The melody is passed to the `setMelody` method as a string where each note is encoded by 3 characters; note length, note value, octave.
For example, "8A4" means en eight-note A4 (440 Hz).
Valid notes are "CdDeEFgGaAbB", with lower case letters being the lower half notes.
Valid scales go from 1 to 7.

## Melody tempo

You can set the playback tempo to a value of beats per minute.
With 120 BPM a quarter note will last for 1 second.

Note that tempo applies only to the next melody set by invoking `setMelody`.

## Getting Started

```
MelodySpeaker speaker = Melody(D1, false); // non-blocking operation

void setup() {
    speaker.begin(); // sets the selected pin to OUTPUT

    speaker.setTempo(240); // optional
    speaker.setMelody("4C7,8G6,8G6,4A6,4G6,4  ,4B6,4C7");
}

void loop() {
    speaker.processMelody();
}
```

## License

MIT license.
