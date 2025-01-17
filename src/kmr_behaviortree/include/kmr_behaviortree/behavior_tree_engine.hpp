// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include <string>
#include <vector>

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/xml_parsing.h"


namespace kmr_behavior_tree
{

enum class BtStatus { SUCCEEDED, FAILED, CANCELED };

class BehaviorTreeEngine
{
public:
  explicit BehaviorTreeEngine(const std::vector<std::string> & plugin_libraries);
  virtual ~BehaviorTreeEngine() {}

  BtStatus run(
    BT::Tree * tree,
    std::function<void()> onLoop,
    std::function<bool()> cancelRequested,
    std::chrono::milliseconds loopTimeout = std::chrono::milliseconds(10));

  BT::Tree buildTreeFromText(
    const std::string & xml_string,
    BT::Blackboard::Ptr blackboard);

  void haltAllActions(BT::TreeNode * root_node)
  {
  // Halts the node
  root_node->halt();
  // backup;
    auto visitor = [](BT::TreeNode * node) {
      if (node->status() == BT::NodeStatus::RUNNING) {
        node->halt();
      }
      // Deprecated version:
        //if (auto action = dynamic_cast<BT::CoroActionNode *>(node)) {
         // action->halt();
        //}
      };
    BT::applyRecursiveVisitor(root_node, visitor);
  }

  // In order to re-run a Behavior Tree, we must be able to reset all nodes to the initial state
  //void resetTree(BT::TreeNode * root_node)
  //{
   // Deprecated, no need to set node status to idle
    //auto visitor = [](BT::TreeNode * node) {
        //node->setStatus(BT::NodeStatus::IDLE);
      //};
    //BT::applyRecursiveVisitor(root_node, visitor);
  //}

protected:
  // The factory that will be used to dynamically construct the behavior tree
  BT::BehaviorTreeFactory factory_;
};

}  // namespace kmr_behavior_tree
