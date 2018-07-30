/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#ifdef FUSION_CONVADDRELU_OP

#include "fusion_conv_add_relu_op.h"
#include "operators/math/conv_func.h"

namespace paddle_mobile {
namespace operators {

template <typename Dtype, typename T>
void FusionConvAddReluOp<Dtype, T>::InferShape() const {
  auto in_dims = this->param_.Input()->dims();
  auto filter_dims = this->param_.Filter()->dims();
  const std::vector<int> &strides = this->param_.Strides();
  std::vector<int> paddings = this->param_.Paddings();
  int groups = this->param_.Groups();
  std::vector<int> dilations = this->param_.Dilations();

  PADDLE_MOBILE_ENFORCE((in_dims.size() == filter_dims.size() &&
                         dilations.size() == paddings.size() &&
                         paddings.size() == strides.size()),
                        "ConvParam is not suitable");

  std::vector<int64_t> output_shape({in_dims[0], filter_dims[0]});
  for (size_t i = 0; i < strides.size(); ++i) {
    output_shape.push_back(
        math::ConvOutputSize(in_dims[i + 2], filter_dims[i + 2], dilations[i],
                             paddings[i], strides[i]));
  }
  framework::DDim ddim = framework::make_ddim(output_shape);
  this->param_.Output()->Resize(ddim);
}

}  // namespace operators
}  // namespace paddle_mobile

namespace ops = paddle_mobile::operators;
#ifdef PADDLE_MOBILE_CPU
REGISTER_OPERATOR_CPU(fusion_conv_add_relu, ops::FusionConvAddReluOp);
#endif
#ifdef PADDLE_MOBILE_MALI_GPU
#endif
#ifdef PADDLE_MOBILE_FPGA
#endif

#endif
