#include "android_renderer_frontend.hpp"

#include <mbgl/renderer/renderer.hpp>
#include <mbgl/map/backend_scope.hpp>
#include <mbgl/util/async_task.hpp>

namespace mbgl {
namespace android {

AndroidRendererFrontend::AndroidRendererFrontend(
        std::unique_ptr<Renderer> renderer_,
        Backend& backend_,
        InvalidateCallback invalidate)
        : renderer(std::move(renderer_))
        , backend(backend_)
        , asyncInvalidate([=, invalidate=std::move(invalidate)]() {
            invalidate();
        }) {
}

AndroidRendererFrontend::~AndroidRendererFrontend() {
    reset();
}

void AndroidRendererFrontend::reset() {
    if (renderer) {
        BackendScope guard { backend };
        renderer.reset();
    }
}

void AndroidRendererFrontend::setObserver(RendererObserver& observer) {
    renderer->setObserver(&observer);
}

void AndroidRendererFrontend::update(std::shared_ptr<UpdateParameters> params) {
    updateParameters = std::move(params);
    asyncInvalidate.send();
}

void AndroidRendererFrontend::render(View& view) {
    assert(BackendScope::exists());

    if (!updateParameters) {
        return;
    }

    renderer->render(backend, view, *updateParameters);
}

std::vector<Feature> AndroidRendererFrontend::queryRenderedFeatures(
        std::shared_ptr<RenderedQueryParameters> params) const {
    return renderer->queryRenderedFeatures(*params);
}

std::vector<Feature> AndroidRendererFrontend::querySourceFeatures(
        std::shared_ptr<SourceQueryParameters> params) const {
    return renderer->querySourceFeatures(*params);
}

void AndroidRendererFrontend::onLowMemory() {
    renderer->onLowMemory();
}

} // namespace android
} // namespace mbgl

