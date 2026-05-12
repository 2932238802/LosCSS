import request from './request'
import { modules } from '../stores/modules'

/**
 * createCrudApi
 *
 * @param {*} baseUrl
 * @returns
 */
export function createCrudApi(baseUrl) {
  return {
    list(params) {
      return request.get(baseUrl, { params })
    },
    detail(id) {
      return request.get(`${baseUrl}/${id}`)
    },
    create(data) {
      return request.post(baseUrl, data)
    },
    update(id, data) {
      return request.put(`${baseUrl}/${id}`, data)
    },
    remove(id) {
      return request.delete(`${baseUrl}/${id}`)
    }
  }
}

export const apiMap = Object.fromEntries(
  Object.entries(modules).map(([key, cfg]) => [key, createCrudApi(cfg.baseApi)])
)

export const seedSourceApi = apiMap.seedSources
export const seedlingApi = apiMap.seedlings
export const plantingAreaApi = apiMap.plantingAreas
export const farmingOperationApi = apiMap.farmingOperations
export const harvestApi = apiMap.harvests
export const rawMaterialInApi = apiMap.rawMaterialIns
export const processingFlowApi = apiMap.processingFlows
export const qualityCheckApi = apiMap.qualityChecks
