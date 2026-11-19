# Stalys (VCV Rack 2 port)

![All Modules](/img/allmodules-hornet.png)

> **This is a fork.** The Stalys modules, panels and documentation were created by [Stalys](https://linktr.ee/stalysmusic) (Denis Dubo Chevalier) in [denischevalier/StalysVCVPlugin](https://github.com/denischevalier/StalysVCVPlugin). This fork ([m9dfukc/StalysVCVPlugin](https://github.com/m9dfukc/StalysVCVPlugin)) only ports the plugin to VCV Rack 2 and Apple Silicon. It adds no new modules or music of its own. Descriptions of the modules and their background below come from the original author.

> **Compatibility:** Version 2.x of this plugin targets **VCV Rack 2** (tested with Rack 2.6.6 on macOS Apple Silicon / arm64). It uses the standard Rack 2 build system, so it should also build on macOS x64, Linux and Windows with the matching Rack SDK. Patches made with the Rack 1 version (1.x) load unchanged, including their panel skin. For VCV Rack 1, use plugin version 1.4.2.

## Abstract

A set of modules exploring [cybernetics](https://en.m.wikipedia.org/wiki/Cybernetics) in music, designed as a [Turing-complete](https://en.m.wikipedia.org/wiki/Turing_completeness) set of instructions for VCV Rack.

## Credits

- Neuron original design: [Andrew Fitch](https://www.nonlinearcircuits.com/)
- Code and Panels: [Stalys](https://linktr.ee/stalysmusic)
- Neuron VCV implementation: [Michael Hetrick](https://mhetrick.com/)
- Befaco slew limiter VCV implementation: [Befaco](https://github.com/VCVRack/Befaco)
- BogAudio skinable module classes: [BogAudio](https://github.com/bogaudio/BogaudioModules)
- VCV Rack 2 / Apple Silicon port: [m9dfukc](https://github.com/m9dfukc/StalysVCVPlugin) (fork)

## Installation

### From a prebuilt package

1. Get the `Stalys-<version>-<os>-<cpu>.vcvplugin` package for your platform (e.g. `Stalys-2.0.0-mac-arm64.vcvplugin` for Apple Silicon Macs), or build it yourself (see below).
2. Copy it into your Rack 2 user plugin folder:
   - macOS (Apple Silicon): `~/Library/Application Support/Rack2/plugins-mac-arm64/`
   - macOS (Intel): `~/Library/Application Support/Rack2/plugins-mac-x64/`
   - Linux: `~/.local/share/Rack2/plugins-lin-x64/`
   - Windows: `%LOCALAPPDATA%\Rack2\plugins-win-x64\`
3. Restart VCV Rack. It unpacks the package on startup, and the modules show up in the module browser under the **Stalys** brand.

### Building from source

**Dependencies:** a C++ toolchain, `make`, `jq` and `zstd`. See the [official VCV Rack manual](https://vcvrack.com/manual/Building#setting-up-your-development-environment) for details. You do not need to build Rack itself. On macOS:

```bash
xcode-select --install
brew install jq zstd
```

**Rack SDK:** download the [Rack 2 SDK](https://vcvrack.com/downloads/) that matches your Rack version and platform, e.g. `Rack-SDK-2.6.6-mac-arm64.zip` for Apple Silicon. The Makefile picks the SDK in this order:

1. `RACK_DIR`, if you set it: `export RACK_DIR=path/to/Rack-SDK`
2. a *built* Rack source tree two directories above the plugin (i.e. the plugin lives in `Rack/plugins/StalysVCVPlugin` and `Rack/libRack.dylib` exists)
3. an SDK unzipped into the plugin directory as `Rack-SDK/` (ignored by git)
4. otherwise `../..`

**Build and install:**

```bash
git clone https://github.com/m9dfukc/StalysVCVPlugin
cd StalysVCVPlugin
make dist      # builds dist/Stalys-<version>-<os>-<cpu>.vcvplugin
make install   # builds and copies the package into your Rack 2 user plugin folder
```

On Apple Silicon the plugin is built natively for arm64 when you use the `mac-arm64` SDK. For an Intel build, use the `mac-x64` SDK.

## Examples

Example patches and real-life uses of this plugin by the original author are in the [/patches](/patches) directory.

## Skins

Stalys modules come with four different skins (since v1.4.0). Right-click a module and open the **Panel** submenu to:

- choose a skin for that module (**Bee**, **Hornet**, **Light**, **Dark**), or **Default** to follow the global default;
- set the global default with **Default to …**. It is stored in `Stalys.json` in your Rack user folder and applies to all modules set to **Default**. Out of the box the default is **Hornet**.

The chosen skin is saved with the patch.


<details><summary>Bee</summary>

![All Modules](/img/allmodules-bee.png)

</details>

<details><summary>Hornet</summary>

![All Modules](/img/allmodules-hornet.png)

</details>

<details><summary>Light</summary>

![All Modules](/img/allmodules-light.png)

</details>

<details><summary>Dark</summary>

![All Modules](/img/allmodules-dark.png)

</details>

## Modules

<details><summary>VC Dual Neuron</summary>

![VC Dual Neuron](/img/vcdualneuron.png)

This module is heavily inspired by [Nonlinear Circuits](https://www.nonlinearcircuits.com/) [Double Neuron](https://github.com/mhetrick/nonlinearcircuits) module.

The module is divided in three parts. On the left are two VC Neurons circuits, and on the right the original Combiner circuit.

Each Neuron is comprised of three lanes: One input, that is multiplied (four-quadrant multiplier). After multiplication, a VC offset is applied. The three inputs are summed and run into a half-wave rectifier and comparator. The comparator's bipolar output is summed with the rectified signal. SENSE adds a positive DC signal to the mix, while RESPONSE affects the magnitude of the comparator's output.

The Combiner circuit consists of two lanes (with four-quadrant multiplication and vc offset), normalized to each Neuron's output. The output of the two lanes runs into several logic modules:

- A Difference Rectifier (DIFFRECT), which is another creative mixer: the two lanes are summed. Then the negative mix is substracted from the positive mix. If the result is positive, it goes out the DIFFRECT+ output, otherwise it goes out the DIFFRECT- output.
- A MAX circuit which outputs the max value out of the two lanes.
- A SUM output which adds the two lanes.
- A DIFF output which substracts the bottom lane from the top lane.
- A MIN circuit which outputs the min value out of the two lanes.

This might seem overwhelming, but just experiment: feed it with LFOs, envelopes, sequences, oscillators or even itself and watch the complex waveforms it generates. You can use just some parts of it and it becomes a mixer/ring-modulator/waveshapper/logic module.

</details>

<details><summary>8-bit Shift Register</summary>

![ASR](/img/asr.png)

This module is an 8 step shift register: At each incoming CLOCK trigger, all output voltages are shifted down one position and the value of IN is sampled into the first output.

Typical use would be to generate canon melodies.

</details>

<details><summary>8 Comparator</summary>

![8 Comparator](/img/comparator.png)

This module is what its name indicates: 8 VC comparators: It outputs a gate whenever the input signal is superior to the value signal (defaults to 0).

Plug two different LFOs to see complex rhythms emerge.

</details>

<details><summary>8 Multiplier</summary>

![8 Multiplier](/img/multiplier.png)

This module is 8 four quadrant multipliers or ring-modulators (with attenuversion of the Carrier). A 5V voltage is normaled to the Carrier input. The Carrier knob goes from -2 to 2. Between -1 and 1, it will act like a normal four-quadrant. Outside those limits, the signal will be amplified up to ~2 times the original amplitude. This allows it to act as a gain stage before a distortion like the **8 Clip** module.

If no cable is plugged into an output, it is summed to the next outputs.

</details>

<details><summary>8 Summing mixer</summary>

![8 Sum](/img/sum.png)

This module is 8 three inputs unity mixer.

If no cable is plugged into an output, it is summed to the next outputs.

</details>

<details><summary>8 Substractor</summary>

![8 Diff](/img/diff.png)

This module is 8 three inputs difference computer.

</details>

<details><summary>8 Not operator</summary>

![8 Not](/img/not.png)

This module is 8 boolean Not operators: they invert the gates they receive as inputs.

</details>

<details><summary>8 Absolute</summary>

![8 Abs](/img/abs.png)

This modules inverses the negative portion of any input voltage.

</details>

<details><summary>8 VC Sample&Delay</summary>

![8 VC Sample&Delay](/img/vcsampleanddelay.png)

This module is 8 sample and hold with a twist: it will only take gate into account every STEP pulses.

This can serve for counterpoint melody generation, VC clock division, or any usual Sample&Hold usages.

</details>

<details><summary>DA Converter</summary>

![DAC](/img/dac.png)

This modules is an 8 bit bipolar digital to analog converter. It converts input triggers and gates into bipolar CV.

One example of use for it is in a Rungler context: Connect it's output to the frequency of a VCO, the inverted output to the frequency of another VCO. Send the output of VCO 1 to the Analog Shift Register, then the outs of the ASR to the DAC, and use the square out of the second VCO as clock for the ASR: You've got a pseudo-random melody generator.

</details>

<details><summary>AD Converter</summary>

![ADC](/img/adc.png)

This modules is an 8 bit bipolar analog to digital converter. It converts input signal into 8 gates. Gates 1-7 represent the number 1 to 127, where as the 8th is the sign of the signal.

Pair it with DAC and invert or mute some gates to get a pretty gnarly bitcrusher/distortion/waveshaper. Or pass any signal to it to generate interesting rhythms.

</details>

<details><summary>8 Max/or operator</summary>

![8 Max](/img/max.png)

This module is 8 times three input MAX operator logic module (ie. Analog OR). Inputs are normalized to -10 volts to permit to not use all. If an output is not plugged, its value is maxed with the next row, thus allowing up to a 24 inputs max computation.

</details>

<details><summary>8 Min/and operator</summary>

![8 Min](/img/min.png)

This module is 8 times three input MIN operator logic module (ie. Analog AND). Inputs are normalized to 10 volts to permit to not use all. If an output is not plugged, its value is minimized with the next row, thus allowing up to a 24 inputs min computation.

</details>

<details><summary>8 Positive/negative rectifier</summary>

![8 Positive/Negative Rectifier](/img/posnegrectifier.png)

This module is 8 rectifiers that output the positive portion of a signal to the **+** output, and the negative to the **-** output.

This can serve to apply different treatment to each half of a waveform or as a distortion effect.

</details>

<details><summary>8 Averager</summary>

![8 Avg](/img/avg.png)

This module is an octal three-inputs averager: It computes the average of its connected inputs. If an output is not connected, it will compute add the corresponding inputs to the average of the following output, thus permitting averaging of up to 24 inputs.

</details>

<details><summary>8 Function generators</summary>

![8 Function](/img/function.png)

This module is 8 function generators. Each row takes (from left to right) one trigger input, one signal input, one rise CV and param, one fall CV and param. It then outputs the slew processed signal, a gate during rising phase, a gate during falling phase and an End-Of-Cycle gate. Note that EOC is up when no cycle (as on a Serge DUSG), to permit feedback.

This module is a jack of all trades. Amongst its various use-cases, here are the main ones:

* AD envelope generator
* LFO
* VCO
* Gate Sequencer
* Rudimentary low-pass gate
* Filter
* Clock generator
* Slew limiter

Inspired by [Befaco Slew Limiter](https://github.com/VCVRack/Befaco/blob/v1/src/SlewLimiter.cpp).

</details>

<details><summary>8 A/B Y switches</summary>

![8 A/B Y](/img/aby.png)

This module is an octal 2-in 1-out switch, eg A/B Y switch.

It can be used to switch between chorus and verse sequences for instance.

</details>

<details><summary>8 Offset</summary>

![8 Offset](/img/offset.png)

This module is an octal offset module, generating voltages from -10 to 10V.

</details>

<details><summary>8 Clip</summary>

![8 Clip](/img/clip.png)

This module is an octal soft-clipping distortion.

Negative ratio will hard clip the signal. Positive ratio will soft clip the signal. Extreme values will fold the signal.

</details>

<details><summary>8 Clock to V/Oct</summary>

![8 C2V/O](img/ctv.png)

This modules transforms a clock input into a Volt/Octave output. Base frequency (0) is 2 hertz.

Use this module to sync LFOs to a clock.

Note that this modules needs at least two clock ticks to accurately compute the frequency. Thus, low BPM clocks will take some time before being accurately converted.

</details>

<details><summary>Blank</summary>

![Blank](img/blank.png)

This is just your regular 24hp blank panel, themable.

</details>

## Support and contributing

**Problems with the Rack 2 port** (building, loading or running in Rack 2) belong in the [issues of this fork](https://github.com/m9dfukc/StalysVCVPlugin/issues). Pull requests are welcome there too.

**Original author:** the modules and their design are by Stalys. If you like them, you can support Stalys on [Patreon](https://www.patreon.com/stalysmusic). The [original repository](https://github.com/denischevalier/StalysVCVPlugin) and its [wiki](https://github.com/denischevalier/StalysVCVPlugin/wiki) remain the reference for the Rack 1 version.
