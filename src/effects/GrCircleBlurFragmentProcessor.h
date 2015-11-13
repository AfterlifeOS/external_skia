/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrCircleBlurFragmentProcessor_DEFINED
#define GrCircleBlurFragmentProcessor_DEFINED

#include "SkTypes.h"

#if SK_SUPPORT_GPU

#include "GrFragmentProcessor.h"
#include "GrProcessorUnitTest.h"

class GrTextureProvider;

// This FP handles the special case of a blurred circle. It uses a 1D 
// profile that is just rotated about the origin of the circle.
class GrCircleBlurFragmentProcessor : public GrFragmentProcessor {
public:
    ~GrCircleBlurFragmentProcessor() override {};

    const char* name() const override { return "CircleBlur"; }

    static const GrFragmentProcessor* Create(GrTextureProvider*textureProvider,
                                             const SkRect& circle, float sigma) {
        float offset;

        SkAutoTUnref<GrTexture> blurProfile(CreateCircleBlurProfileTexture(textureProvider,
                                                                           circle,
                                                                           sigma,
                                                                           &offset));
        if (!blurProfile) {
           return nullptr;
        }
        return new GrCircleBlurFragmentProcessor(circle, sigma, offset, blurProfile);
    }

    const SkRect& circle() const { return fCircle; }
    float sigma() const { return fSigma; }
    float offset() const { return fOffset; }
    int profileSize() const { return fBlurProfileAccess.getTexture()->width(); }

private:
    GrCircleBlurFragmentProcessor(const SkRect& circle, float sigma,
                                  float offset, GrTexture* blurProfile);

    GrGLSLFragmentProcessor* onCreateGLInstance() const override;

    void onGetGLProcessorKey(const GrGLSLCaps& caps, GrProcessorKeyBuilder* b) const override;

    bool onIsEqual(const GrFragmentProcessor& other) const override {
        const GrCircleBlurFragmentProcessor& cbfp = other.cast<GrCircleBlurFragmentProcessor>();
        // fOffset is computed from the circle width and the sigma
        return this->circle().width() == cbfp.circle().width() && fSigma == cbfp.fSigma;    
    }

    void onComputeInvariantOutput(GrInvariantOutput* inout) const override;

    static GrTexture* CreateCircleBlurProfileTexture(GrTextureProvider*,
                                                     const SkRect& circle,
                                                     float sigma, float* offset);

    SkRect              fCircle;
    float               fSigma;
    float               fOffset;
    GrTextureAccess     fBlurProfileAccess;

    GR_DECLARE_FRAGMENT_PROCESSOR_TEST;

    typedef GrFragmentProcessor INHERITED;
};

#endif
#endif
