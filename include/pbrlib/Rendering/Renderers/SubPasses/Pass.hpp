//
//  Pass.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/09/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Pass_hpp
#define Pass_hpp

#include <memory>

using namespace std;

namespace pbrlib
{
    class ImageView;
    class RenderPass;

    using PtrImageView  = shared_ptr<ImageView>;
    using PtrRenderPass = shared_ptr<RenderPass>;

    /**
     * @interface IPassOutput.
     * @details 
     *      Этот интерфейс требует реализации следующих методов:
     *          1) const ImageView& outputImpl(size_t id)                                   ;
     *          2) void             outputImpl(PtrImageView& ptr_image_view, size_t id)     ;
     *          3) void             outputImpl(ImageView& image_view, size_t id)            .
     * 
     * @tparam PassType тип прохода, реализующий данный интерфейс.
    */
    template<typename PassType>
    class IPassOutput
    {
    public:
        IPassOutput() = default;

        IPassOutput(IPassOutput&&)      = delete;
        IPassOutput(const IPassOutput&) = delete;

        IPassOutput& operator = (IPassOutput&&)         = delete;
        IPassOutput& operator = (const IPassOutput&)    = delete;

        const ImageView& output(size_t id)
        {
            return static_cast<const PassType*>(this)->outputImpl(id);
        }

        void output(PtrImageView& ptr_image_view, size_t id)
        {
            static_cast<PassType*>(this)->outputImpl(ptr_image_view, id);
        }

        void output(ImageView& image_view, size_t id)
        {
            static_cast<PassType*>(this)->outputImpl(image_view, id);
        }
    };

    /**
     * @interface IPassInputOutput.
     * @details 
     *      Необходимо реализовать методы, прописанные в описании интерфейса IPassOutput. Помимо этого,
     *      нужно ещё реализовать следующий метод:
     *          void inputImpl(const PtrImageView& ptr_image_view, size_t id)   .
     * 
     * @tparam PassType тип прохода, реализующий данный интерфейс.
    */
    template<typename PassType>
    class IPassInputOutput :
        public IPassOutput<PassType>
    {
    public:
        IPassInputOutput() = default;

        IPassInputOutput(IPassInputOutput&&)        = delete;
        IPassInputOutput(const IPassInputOutput&)   = delete;

        IPassInputOutput& operator = (IPassInputOutput&&)       = delete;
        IPassInputOutput& operator = (const IPassInputOutput&)  = delete;

        void input(const PtrImageView& ptr_image_view, size_t id)
        {
            static_cast<PassType*>(this)->inputImpl(ptr_image_view, id);
        }
    };
}

#endif /* Pass_hpp */
