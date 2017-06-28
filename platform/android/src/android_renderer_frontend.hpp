
#pragma once

#include <mbgl/map/backend.hpp>
#include <mbgl/renderer/renderer_frontend.hpp>
#include <mbgl/util/async_task.hpp>

#include <functional>
#include <memory>

namespace mbgl {

class Renderer;

namespace android {

class AndroidRendererFrontend : public RendererFrontend {
public:
    using InvalidateCallback = std::function<void ()>;
    AndroidRendererFrontend(std::unique_ptr<Renderer>, Backend&, InvalidateCallback);
    ~AndroidRendererFrontend() override;

    void reset() override;
    void setObserver(RendererObserver&) override;

    void update(std::shared_ptr<UpdateParameters>) override;
    void render(View& view);

    std::vector<Feature> queryRenderedFeatures(std::shared_ptr<RenderedQueryParameters>) const override;
    std::vector<Feature> querySourceFeatures(std::shared_ptr<SourceQueryParameters>) const override;

    // Memory
    void onLowMemory();

private:
    std::unique_ptr<Renderer> renderer;
    Backend& backend;
    std::shared_ptr<UpdateParameters> updateParameters;
    util::AsyncTask asyncInvalidate;
};

} // namespace android
} // namespace mbgl
