Performs basic post-processing with a built-in set of shaders which operate using two input textures.

See also [[PostProcess]]

# Header

    #include "PostProcess.h"

# Initialization
Construction requires a Direct3D 11 device.

    std::unique_ptr<DualPostProcess> postProcess;
    postProcess = std::make_unique<DualPostProcess>(device);

For exception safety, it is recommended you make use of the C++ [RAII](http://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) pattern and use a ``std::unique_ptr`` or ``std::shared_ptr``

# Shader selection

DualPostProcess can be configured to use various pixel shaders by calling ``SetEffect``:

* **Merge**: Combines two images using provided weights.
* **BloomCombine**: Combines the bloom texture produced by [[BasicPostProcess]] in a final pass with the original scene texture.
    
# Properties

* ``SetSourceTexture``, ``SetSourceTexture2`` is used to set the input texture as a Shader Resource View.

* ``SetMergeParameters`` is used to set the two weights for the _Merge_ shader. Setting these to 0.5,0.5 will blend two images, while setting them both to 1.0 will add the two images together.

* ``SetBloomCombineParameters`` sets the various parameters for the _BloomCombine_ shader as the final pass of implementing a bloom effect.