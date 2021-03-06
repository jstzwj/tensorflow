
/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/c/experimental/ops/nn_ops.h"

#include "tensorflow/c/eager/tracing_utils.h"
#include "tensorflow/core/platform/errors.h"

using tensorflow::tracing::MaybeSetOpName;

namespace tensorflow {
namespace ops {

// Softmax Loss given scores and labels, used by the SoftMaxLossGradient
Status SparseSoftmaxCrossEntropyWithLogits(
    AbstractContext* ctx, AbstractTensorHandle* const features,
    AbstractTensorHandle* const labels,
    absl::Span<AbstractTensorHandle*> outputs, const char* name) {
  AbstractOperationPtr op_ptr(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(op_ptr->Reset("SparseSoftmaxCrossEntropyWithLogits",
                                   /*raw_device_name=*/nullptr));
  TF_RETURN_IF_ERROR(MaybeSetOpName(op_ptr.get(), name));
  TF_RETURN_IF_ERROR(op_ptr->AddInput(features));  // input scores
  TF_RETURN_IF_ERROR(op_ptr->AddInput(labels));    // labels

  // Outputs will contain: [loss_vals, gradients].
  int num_retvals = 2;
  TF_RETURN_IF_ERROR(op_ptr->Execute(outputs, &num_retvals));
  return Status::OK();
}

// Computes Relu gradient given input features
Status ReluGrad(AbstractContext* ctx, AbstractTensorHandle* const gradients,
                AbstractTensorHandle* const features,
                absl::Span<AbstractTensorHandle*> backprops, const char* name) {
  AbstractOperationPtr op_ptr(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(op_ptr->Reset("ReluGrad", /*raw_device_name=*/nullptr));
  TF_RETURN_IF_ERROR(MaybeSetOpName(op_ptr.get(), name));
  TF_RETURN_IF_ERROR(op_ptr->AddInput(gradients));  // upstream grads
  TF_RETURN_IF_ERROR(op_ptr->AddInput(features));   // relu inputs

  int num_retvals = 1;
  TF_RETURN_IF_ERROR(op_ptr->Execute(backprops, &num_retvals));
  return Status::OK();
}

Status Relu(AbstractContext* ctx, AbstractTensorHandle* const features,
            absl::Span<AbstractTensorHandle*> activations, const char* name) {
  AbstractOperationPtr op_ptr(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(op_ptr->Reset("Relu", /*raw_device_name=*/nullptr));
  TF_RETURN_IF_ERROR(MaybeSetOpName(op_ptr.get(), name));
  TF_RETURN_IF_ERROR(op_ptr->AddInput(features));

  int num_retvals = 1;
  TF_RETURN_IF_ERROR(op_ptr->Execute(activations, &num_retvals));
  return Status::OK();
}

Status BiasAdd(AbstractContext* ctx, AbstractTensorHandle* const value,
               AbstractTensorHandle* const bias,
               absl::Span<AbstractTensorHandle*> output, const char* name,
               const char* data_format) {
  AbstractOperationPtr op_ptr(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(op_ptr->Reset("BiasAdd", /*raw_device_name=*/nullptr));
  TF_RETURN_IF_ERROR(MaybeSetOpName(op_ptr.get(), name));
  TF_RETURN_IF_ERROR(op_ptr->AddInput(value));  // tensor input
  TF_RETURN_IF_ERROR(op_ptr->AddInput(bias));   // bias

  int num_retvals = 1;
  TF_RETURN_IF_ERROR(op_ptr->Execute(output, &num_retvals));
  return Status::OK();
}

// Computes Bias Add gradient given upstream grads
Status BiasAddGrad(AbstractContext* ctx,
                   AbstractTensorHandle* const out_backprop,
                   absl::Span<AbstractTensorHandle*> output, const char* name,
                   const char* data_format) {
  AbstractOperationPtr op_ptr(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(op_ptr->Reset("BiasAddGrad", /*raw_device_name=*/nullptr));
  TF_RETURN_IF_ERROR(MaybeSetOpName(op_ptr.get(), name));
  TF_RETURN_IF_ERROR(
      op_ptr->SetAttrString("data_format", data_format, strlen(data_format)));
  TF_RETURN_IF_ERROR(op_ptr->AddInput(out_backprop));

  int num_retvals = 1;
  TF_RETURN_IF_ERROR(op_ptr->Execute(output, &num_retvals));
  return Status::OK();
}

}  // namespace ops
}  // namespace tensorflow
