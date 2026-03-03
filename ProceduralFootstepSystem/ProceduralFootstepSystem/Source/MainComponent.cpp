#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);


    //----------------------------------------------------
    //GUI Setup

    //Sliders
	setupSlider(speedSlider, speedLabel, "Speed m/s", 0.0, 5.0, 2.5);
	setupSlider(weightSlider, weightLabel, "Weight", 0.0, 1.0, 0.7);
	setupSlider(clothSlider, clothLabel, "Cloth Intensity", 0.0, 1.0, 0.4);

	//ComboBox for Surface Type

    //CoboBox Label
	surfaceTypeLabel.setText("Surface Type", juce::dontSendNotification);
	surfaceTypeLabel.setJustificationType(juce::Justification::centredLeft);
	surfaceTypeLabel.attachToComponent(&surfaceTypeBox, true);
	addAndMakeVisible(surfaceTypeLabel);

	//ComboBox Items    
    surfaceTypeBox.addItem("Concrete", 1);
    surfaceTypeBox.addItem("Grass", 2);
    surfaceTypeBox.addItem("Metal", 3);
    surfaceTypeBox.addItem("Water", 4);
    surfaceTypeBox.setSelectedId(1);
    surfaceTypeBox.addListener(this);
	addAndMakeVisible(surfaceTypeBox);

	//Visual Feedback Labels and Display
    setupDisplayLabel(gaitTypeLabel, "Gait: Walk");
    setupDisplayLabel(stepPhaseLabel, "0.00");
    setupDisplayLabel(activeVoicesLabel, "Active Voices: 0");
    setupDisplayLabel(currentStatusLabel, "Status: Idle");



    //Placeholder logic for update parameters from GUI
    footstepParameters.speed.store(2.5f, std::memory_order_relaxed);
    footstepParameters.weight.store(0.7f, std::memory_order_relaxed);
    footstepParameters.clothIntensity.store(0.6f, std::memory_order_relaxed);
    footstepParameters.surfaceType.store(0, std::memory_order_relaxed);

    //DBG("MainComponent Constructor - Parameters Set");
    //DBG("Speed: 2.5, Weight: 0.7, Cloth: 0.4, Surface: 0");

	// Start Timer for GUI updates at 30 Hz
	startTimer(30);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//Helper functions for GUI setup

void MainComponent::setupSlider(juce::Slider& slider, juce::Label& label, 
                                const juce::String& labelText, 
                                double min, double max, double defaultValue)
{
    // Setup Slider
	slider.setSliderStyle(juce::Slider::LinearHorizontal);
	slider.setRange(min, max, 0.01);
	slider.setValue(defaultValue);
	slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
	slider.addListener(this);
	addAndMakeVisible(slider);

	// Setup Label
	label.setText(labelText, juce::dontSendNotification);
	label.setJustificationType(juce::Justification::centredLeft);
	label.attachToComponent(&slider, true);
	addAndMakeVisible(label);
}

void MainComponent::setupDisplayLabel(juce::Label& label, const juce::String& initialText)
{
	label.setText(initialText, juce::dontSendNotification);
	label.setJustificationType(juce::Justification::centredLeft);
	label.setFont(juce::Font(16.0f, juce::Font::bold));
	label.setColour(juce::Label::textColourId, juce::Colours::lightblue);
	addAndMakeVisible(label);
}

//Sliders Callbacks - Update Parameters
void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &speedSlider)
    {
		float speed = static_cast<float>(slider->getValue());
		footstepParameters.speed.store(speed, std::memory_order_relaxed);
    }

    else if (slider == &weightSlider)
	{
        float weight = static_cast<float>(slider->getValue());
        footstepParameters.weight.store(weight, std::memory_order_relaxed);
    }
    else if (slider == &clothSlider)
    {
        float clothIntensity = static_cast<float>(slider->getValue());
        footstepParameters.clothIntensity.store(clothIntensity, std::memory_order_relaxed);
    }
}

//ComboBox Callback - Update Surface Type
void MainComponent::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &surfaceTypeBox)
    {
		int selectedId = comboBox->getSelectedId();
		//ComboBox IDs start from 1, so we subtract 1 to match our enum indexing
		int surfaceIndex = selectedId - 1;

		footstepParameters.surfaceType.store(surfaceIndex, std::memory_order_relaxed);
	}
}

//Timer Callback - Update GUI Displays
void MainComponent::timerCallback()
{
	//Update Gait Type Display
	int gait = currentGaitType.load(std::memory_order_relaxed);
	juce::String gaitText;
    switch (gait)
    {
    case 0: gaitText = "Sneak"; break;
	case 1: gaitText = "Walk"; break;
	case 2: gaitText = "Run"; break;
	default: gaitText = "Gait: Unknown"; break;
    }

	gaitTypeLabel.setText("Gait: " + gaitText, juce::dontSendNotification);

	//Update Step Phase Display
	float phase = currentStepPhase.load(std::memory_order_relaxed);
	stepPhaseLabel.setText("Step Phase: " + juce::String(phase, 2), juce::dontSendNotification);

	//Update Active Voices Display
	int voices = activeVoiceCount.load(std::memory_order_relaxed);
	activeVoicesLabel.setText("Active Voices: " + juce::String(voices), juce::dontSendNotification);

	//Update Current Status based on Activity
	float currentSpeed = footstepParameters.speed.load(std::memory_order_relaxed);
    if(currentSpeed < 0.05f)
		currentStatusLabel.setText("Status: Idle", juce::dontSendNotification);
    else
		currentStatusLabel.setText("Status: Active", juce::dontSendNotification);
}



//JUCE Standard AudioAppComponent Methods
//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    DBG("=== prepareToPlay CALLED ===");
    DBG("Sample Rate: " + juce::String(sampleRate));
    DBG("Samples Per Block: " + juce::String(samplesPerBlockExpected));

	currentBlockSize = samplesPerBlockExpected;
	currentSampleRate = sampleRate;

	footstepSystem.prepare(currentSampleRate, currentBlockSize);

    DBG("FootstepSystem prepared successfully");

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();

	footstepSystem.process(*bufferToFill.buffer, footstepParameters);

    // Update feedback data for visual display (thread-safe with atomics)
    currentGaitType.store(static_cast<int>(footstepSystem.getCurrentGaitType()),
        std::memory_order_relaxed);
    currentStepPhase.store(footstepSystem.getStepPhase(),
        std::memory_order_relaxed);
    activeVoiceCount.store(footstepSystem.getActiveVoiceCount(),
        std::memory_order_relaxed);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}



//GUI Layout
//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour(0xff0c3046));

    // Title
	g.setColour(juce::Colours::white);
	g.setFont(24.0f);
	g.drawText("Procedural Footstep System", 10, 10, getWidth() - 20, 30, juce::Justification::centred);

    //Section Headers
	g.setFont(16.0f);
	g.setColour(juce::Colours::lightgrey);
	g.drawText("Controls", 10, 50, 150, 20, juce::Justification::centredLeft);
	g.drawText("Status", 10, 330, 150, 20, juce::Justification::centredLeft);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	//Layout constrants
    const int margin = 20;
	const int labelWidth = 120;
	const int sliderHeight = 30;
	const int controlsTop = 80;
	const int spacing = 50;

	//Control Sliders
	int yPos = controlsTop;

	speedSlider.setBounds(margin + labelWidth, yPos, getWidth() - 2 * margin - labelWidth, sliderHeight);
	yPos += spacing;

	weightSlider.setBounds(margin + labelWidth, yPos, getWidth() - 2 * margin - labelWidth, sliderHeight);
	yPos += spacing;

	clothSlider.setBounds(margin + labelWidth, yPos, getWidth() - 2 * margin - labelWidth, sliderHeight);
    yPos += spacing;

    //Surface Type Selector
	surfaceTypeBox.setBounds(margin + labelWidth, yPos, 150, sliderHeight);
	yPos += spacing;

	//Status Labels
	yPos = 360;

	const int feedbackHeight = 30;

	gaitTypeLabel.setBounds(margin, yPos, getWidth() - 2 * margin, feedbackHeight);
	yPos += feedbackHeight + 10;

	stepPhaseLabel.setBounds(margin, yPos, getWidth() - 2 * margin, feedbackHeight);
	yPos += feedbackHeight + 10;

	activeVoicesLabel.setBounds(margin, yPos, getWidth() - 2 * margin, feedbackHeight);
	yPos += feedbackHeight + 10;

	currentStatusLabel.setBounds(margin, yPos, getWidth() - 2 * margin, feedbackHeight);
	//yPos += feedbackHeight + 10;
}
