import request from './request'

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

export const seedSourceApi = createCrudApi('/seed-sources')
export const seedlingApi = createCrudApi('/seedlings')
export const plantingAreaApi = createCrudApi('/planting-areas')
export const farmingOperationApi = createCrudApi('/farming-operations')
export const harvestApi = createCrudApi('/harvests')
export const rawMaterialInApi = createCrudApi('/raw-material-ins')
export const processingFlowApi = createCrudApi('/processing-flows')
export const qualityCheckApi = createCrudApi('/quality-checks')
