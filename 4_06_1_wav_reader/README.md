# 🎶 4.06. Assignment: WAV Reader 🎼

> [!NOTE]
> In the _solution_ folder, you will find an example solution. In the _solution/data_ folder, you will find example WAV files to test your program on.

Your assignment for this lesson is very important. It will teach you a lot about the WAV format, the C++ programming language, bit manipulation, and audio processing in general. That’s why there’s no quiz for this lesson: this assignment suffices.
Your assignment is to write a WAV file reader in C++. Your reader should:

1. display all the metadata contained in the header of the WAV file,
2. print the header and the first 100 samples in the HEX form to the standard output,
3. print the header and the first 100 samples interpreted as ASCII codes to the standard output; display non-printable characters as dots,
4. convert the stored samples to the [-1, 1] range using the conversion mechanisms you learned in lessons 1.09 and 4.05,
5. write the converted samples to a raw binary file and save it to your disk.

You’ll know you’ve successfully completed the assignment if:

1. the displayed metadata is correct,
2. in the HEX form, you can find hex numbers standing for “RIFF” and “WAVE” codes,
3. in the ASCII form, you can see “RIFF”, “WAVE”, “fmt “, and “data” chunk IDs,
4. you can import the binary data in Audacity using *File → Import Audio → Raw Data,* specifying correct parameters specific to the raw samples.

You only need to handle 8-bit PCM and 16-bit PCM samples, along with mono and stereo channels. Error handling does not have to be very robust, so don’t let that hold you back! You can put all of your code into a single source file to be compiled with a C++ compiler. Don’t use any third-party libraries. You can use the WAV files attached to this lesson for testing. If you get stuck, don’t hesitate to search for help online—it’s normal to need reference material for this kind of task!

You can also check out JUCE’s WAV reader or the AudioFile library for reference. In the next lesson, I’ll provide a sample solution for this assignment. But before checking it, try to complete the task on your own first, dedicating at least 3-4 hours of effort.

If you have any questions, don’t hesitate to ask them in the comments. I am notified upon each comment and will try to answer as soon as possible.

I promise, your hard work on this assignment will pay off, and the satisfaction of completing it will be immense!

