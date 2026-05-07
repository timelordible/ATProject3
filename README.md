## Project Objectives:

To create a flanger that works off open in any DAW using only C++ code. It works by taking user input from a custom GUI that is then converted into parameter values for the effect to be built around. Since this is a flanger effect specifically, we found set parameter value ranges from a number of previously designed flanger effects. This ensured that the flanger effect specifically was generated no matter what input the users chose for specific values. Once passed into the plugin processor, the parameter values are smoothed and sent directly to the delay class where they are then updated in this class to be further processed. Finally, the process block in the delay class takes in all of this information and creates a delayed signal that is called in the processBlock in the plugin processor. Our GUI includes rotary sliders for all the main parameters which is attached using APVTS, a styled toggle button, a custom color palette. This interface was based off of the Lecture 9 GUI structure with improved aesthetics and interaction. 


## AI Statement
AI was used to assist in 
- Explaining new GUI functions and customizable components like TextButton, ColourGradient, and setFont()
- helping identify syntax and logic errors in DSP and GUI code
- finding initial parameter ranges for flanger effects 
