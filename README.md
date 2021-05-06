# HyperAugment

## Description

**What is HyperAugment**
HyperAugment is an image data augmentation algorithm, meaning you give it a dataset of images and it will multipply and diversify the images in the dataset. The method that HyperAugment uses to augment images is highly experimental, it has show promising results in plant identification tasks but much still needs to be understood of how models trained on data augmented by HyperAugment behave.

**Why it was created**
Hyper augment is meant to be used my machines not by humans, the idea is that if a robot can segment out an object of interest, either using color or a semantic segmentaion model, then the segmented images can be fed to HyperAugment to generate a large dataset persenting the object in many different environments, and the robot can use this dataset to learn to recognize an object on the spot.

**How does it work**

*refer to the developer docs for more details*

HyperAugment follows this basic diagram:

![basic diagram](./docs/images/augment-flow.jpg)





# This project is not completed, there is no working software currently
