import { TreeNode } from "../TreeNode";

export interface NodeAny{
    key: any
    pointer: TreeNode
    less(x: NodeAny): boolean
    equal(x: NodeAny): boolean
}